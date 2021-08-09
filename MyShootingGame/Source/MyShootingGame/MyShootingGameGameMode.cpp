// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyShootingGameGameMode.h"
#include "MyShootingGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectileActor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileActorBuilder.h"

AMyShootingGameGameMode::AMyShootingGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	mIsTouchedFire = false;
	mIsTouchedSpecialFire = false;

	mIsReleasedFire = false;
	mIsReleassedSpecialFire = false;

	mWhenTouchedFireTimeSec = 0.f;
	mActionBuilder = NewObject<UProjectileActorBuilder>(this, UProjectileActorBuilder::StaticClass(), TEXT("ActionBuilder"));
}

void AMyShootingGameGameMode::StartPlay()
{
	Super::StartPlay();
	mActionBuilder->InitBuilder(this);
}

void AMyShootingGameGameMode::TouchFireStart()
{
	if (!mIsTouchedFire)
	{
		mIsTouchedFire = true;
		mWhenTouchedFireTimeSec = UGameplayStatics::GetTimeSeconds(this);

		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		mActionBuilder->TriggerCondition(Pawn);
	}
}

void AMyShootingGameGameMode::TouchFireRelease()
{
	if (mIsTouchedFire)
	{
		mIsTouchedFire = false;
		mIsReleasedFire = true;

		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		mActionBuilder->TriggerCondition(Pawn);

		mWhenTouchedFireTimeSec = 0;
		mIsReleasedFire = false;
	}
}

void AMyShootingGameGameMode::TouchSpecialFireStart()
{

	if (!mIsTouchedSpecialFire)
	{
		mIsTouchedSpecialFire = true;
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		mActionBuilder->TriggerCondition(Pawn);
	}
}

void AMyShootingGameGameMode::TouchSpecialFireRelease()
{
	if (mIsTouchedSpecialFire)
	{
		mIsTouchedSpecialFire = false;
		mIsReleassedSpecialFire = true;

		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		mActionBuilder->TriggerCondition(Pawn);

		mIsReleassedSpecialFire = false;
	}
}

void AMyShootingGameGameMode::SpawnProjectile(FString InPath, const FProjectileParam& InParam)
{
	UClass* ProjectileClass = StaticLoadClass(AProjectileActor::StaticClass(), nullptr, *InPath);

	FTransform Tr(InParam.mDir.Rotation(), InParam.mInitLoc, InParam.mInitScale3D);
	AProjectileActor* ProjectileActor = Cast<AProjectileActor>(GetWorld()->SpawnActor(ProjectileClass, &Tr));

	if(ProjectileActor)
		ProjectileActor->InitProjectile( InParam);
}

float AMyShootingGameGameMode::GetFirePressedTime() const
{
	float whenReleasedFireTimeSec = UGameplayStatics::GetTimeSeconds(this);
	return whenReleasedFireTimeSec - mWhenTouchedFireTimeSec;
}
