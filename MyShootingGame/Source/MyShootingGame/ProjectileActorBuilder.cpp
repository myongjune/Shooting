// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActorBuilder.h"
#include "MyShootingGameGameMode.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "ProjectileActor.h"

UProjectileActorBuilder::UProjectileActorBuilder(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UProjectileActorBuilder::InitBuilder(class AMyShootingGameGameMode* InGameMode)
{	
	mGameMode = InGameMode;

	LoadAnimFile(TEXT("NonUFS/ProjectileAnimNormal.json"));
	LoadAnimFile(TEXT("NonUFS/ProjectileAnimCharge.json"));
	LoadAnimFile(TEXT("NonUFS/ProjectileAnimDivide.json"));
	LoadAnimFile(TEXT("NonUFS/ProjectileAnimReflect.json"));
}

void UProjectileActorBuilder::TriggerCondition(AActor* InTriggingActor)
{
	for (const FProjectileAnim& ProjAnim : mAnimList)
	{
		// 모든 조건이 맞아야, Exec를 실행한다.
		if (_CheckCondition(InTriggingActor, ProjAnim.ConditionList))
			_Exec(InTriggingActor, ProjAnim.ExecList);
	}
}

void UProjectileActorBuilder::LoadAnimFile(FString InFileRefPath)
{
	FString jsonStr;
	FString FullPath = FPaths::ProjectContentDir();
	FFileHelper::LoadFileToString(jsonStr, *(FullPath + InFileRefPath));

	TSharedPtr<FJsonObject> jsonObj(new FJsonObject());
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(jsonStr);

	if (!FJsonSerializer::Deserialize(Reader, jsonObj))	
		return;


	const TArray<TSharedPtr<FJsonValue>>& ListArray = jsonObj->GetArrayField(TEXT("list"));
	for (auto& JsonAnim : ListArray) 
	{
		FProjectileAnim ProjAnim;
		
		// Condition
		const TSharedPtr<FJsonObject>& Obj = JsonAnim->AsObject();
		const TArray<TSharedPtr<FJsonValue>>& CondArray = Obj->GetArrayField(TEXT("cond"));

		for (auto& CondJson : CondArray) 
		{
			FConditionProjectile ProjCond;
			const TSharedPtr<FJsonObject>& CondObj = CondJson->AsObject();
			ProjCond.ConditionType = CondObj->GetStringField(TEXT("type"));

			CondObj->TryGetNumberField(TEXT("min"), ProjCond.ConditionMin);
			CondObj->TryGetNumberField(TEXT("max"), ProjCond.ConditionMax);
			CondObj->TryGetNumberField(TEXT("index"), ProjCond.ConditionIndex);
			ProjCond.ConditionBlockDestory = CondObj->HasField(TEXT("blockDestroy")) ? CondObj->GetBoolField(TEXT("blockDestroy")) : false;

			ProjAnim.ConditionList.Add(ProjCond);
		}

		// Execute
		const TArray<TSharedPtr<FJsonValue>>& ExecArray = Obj->GetArrayField(TEXT("exec"));
		for (auto& ExecJson : ExecArray)
		{
			FExecuteProjectile ProjExec;
			const TSharedPtr<FJsonObject>& ExecObj = ExecJson->AsObject();
			ProjExec.ExecType = ExecObj->GetStringField(TEXT("type"));
			ExecObj->TryGetNumberField(TEXT("index"), ProjExec.ExecIndex);
			ExecObj->TryGetNumberField(TEXT("lifetime"), ProjExec.ExecLifeTime);

			ExecObj->TryGetNumberField(TEXT("speed"), ProjExec.ExecSpeed);
			ExecObj->TryGetNumberField(TEXT("scale"), ProjExec.ExecScale);
			ExecObj->TryGetNumberField(TEXT("angle"), ProjExec.ExecAngle);
			ExecObj->TryGetNumberField(TEXT("LocDirOffset"), ProjExec.ExecLocDirOffset);
			ExecObj->TryGetNumberField(TEXT("LocHeightOffset"), ProjExec.ExecHeightOffset);

			int r = 0, g = 0, b = 0;
			ExecObj->TryGetNumberField(TEXT("r"), r);
			ExecObj->TryGetNumberField(TEXT("g"), g);
			ExecObj->TryGetNumberField(TEXT("b"), b);

			ProjExec.ExecColor.R = r;
			ProjExec.ExecColor.G = g;
			ProjExec.ExecColor.B = b;
			ProjExec.ExecColor.A = 255.f;
			ProjAnim.ExecList.Add(ProjExec);
		}


		mAnimList.Add(ProjAnim);
	}
}

bool UProjectileActorBuilder::_CheckCondition(class AActor* InTriggingActor, const TArray<FConditionProjectile>& InCondList)
{
	for (const auto& ProjCond : InCondList)
	{
		if (ProjCond.ConditionType == TEXT("FirePressed"))
		{
			if (!mGameMode->IsFirePressed())
				return false;
		}
		if (ProjCond.ConditionType == TEXT("SpecialFirePressed"))
		{
			if (!mGameMode->IsSpecialFirePressed())
				return false;
		}

		if (ProjCond.ConditionType == TEXT("FireReleased"))
		{
			if (!mGameMode->IsFireReleased())
				return false;
		}
		if (ProjCond.ConditionType == TEXT("SpecialFireReleased"))
		{
			if (!mGameMode->IsSpecialFireReleased())
				return false;
		}

		if (ProjCond.ConditionType == TEXT("FriePressedTime"))
		{
			const float pressedTime = mGameMode->GetFirePressedTime();
			if (0 < ProjCond.ConditionMin)
			{
				if (pressedTime < ProjCond.ConditionMin)
					return false;
			}

			if (0 < ProjCond.ConditionMax)
			{
				if (ProjCond.ConditionMax < pressedTime)
					return false;
			}
		}

		if (ProjCond.ConditionType == TEXT("DestroyProjectile"))
		{
			AProjectileActor* ProjActor = Cast<AProjectileActor>(InTriggingActor);
			if (!ProjActor) return false;
			if (!ProjActor->IsWaitDestroy()) return false;

			if (ProjCond.ConditionBlockDestory) 
			{
				if (!ProjActor->IsDestroyCauseBlock()) return false;
			}
		}

		if (ProjCond.ConditionType == TEXT("CompareIndex"))
		{
			AProjectileActor* ProjActor = Cast<AProjectileActor>(InTriggingActor);
			if (!ProjActor) return false;
			if (ProjActor->GetProjectileParam().mIndex != ProjCond.ConditionIndex) return false;
		}
	}

	return true;
}

void UProjectileActorBuilder::_Exec(class AActor* InTriggingActor, const TArray< FExecuteProjectile>& InExecList)
{
	for (auto& ProjExec : InExecList)
	{
		if (ProjExec.ExecType == TEXT("Projectile"))
		{
			FProjectileParam	projectileParam;

			FVector ActorLoc = InTriggingActor->GetActorLocation();
			FRotator ActorRot = InTriggingActor->GetActorRotation();
			FVector ActorDir = ActorRot.Vector();

			projectileParam.mIndex = ProjExec.ExecIndex;
			projectileParam.mInitLoc = ActorLoc + (ActorDir * ProjExec.ExecLocDirOffset);
			projectileParam.mInitLoc.Z += ProjExec.ExecHeightOffset;

			projectileParam.mInitScale3D = FVector(ProjExec.ExecScale, ProjExec.ExecScale, ProjExec.ExecScale);
			projectileParam.mDir = ActorDir.RotateAngleAxis(ProjExec.ExecAngle, FVector::UpVector);;

			projectileParam.mLifeTime = ProjExec.ExecLifeTime;
			projectileParam.mSpeed = ProjExec.ExecSpeed;
			projectileParam.mColor = ProjExec.ExecColor;

			mGameMode->SpawnProjectile(TEXT("/Game/SideScrollerCPP/Blueprints/ProjectileActor_Base.ProjectileActor_Base_C"), projectileParam);
		}

		else if (ProjExec.ExecType == TEXT("UiCount"))
		{
			mGameMode->IncreaseProjCount(ProjExec.ExecIndex);
		}

		else if (ProjExec.ExecType == TEXT("HideCharging"))
		{
			mGameMode->HideCharging();
		}

		else if (ProjExec.ExecType == TEXT("IncFireCharging"))
		{
			const float MaxTime = ProjExec.ExecLifeTime;
			mGameMode->IncreaseCharging(MaxTime);
		}
	}
}
