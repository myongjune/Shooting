// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectileActorBuilder.generated.h"

/**
 * 
 */

USTRUCT()
struct FConditionProjectile 
{
	GENERATED_BODY()

	FString		ConditionType;
	int				ConditionMin = -1;
	int				ConditionMax = -1;
	int				ConditionIndex = -1;

	bool			ConditionBlockDestory = false;
};

USTRUCT()
struct FExecuteProjectile 
{
	GENERATED_BODY()

	FString		ExecType;
	int				ExecIndex;
	
	double			ExecLifeTime;
	double			ExecSpeed;
	double			ExecScale;
	double			ExecAngle;
	double			ExecLocDirOffset;
	double			ExecHeightOffset;
	FColor			ExecColor;

	FExecuteProjectile()
		: ExecIndex(0),
		ExecLifeTime(0),
		ExecSpeed(0),
		ExecScale(0),
		ExecAngle(0),
		ExecLocDirOffset(0),
		ExecHeightOffset(0)
	{}
};

USTRUCT()
struct FProjectileAnim
{
	GENERATED_BODY()

	TArray<FConditionProjectile>	ConditionList;
	TArray<FExecuteProjectile>	ExecList;
};

UCLASS()
class MYSHOOTINGGAME_API UProjectileActorBuilder : public UObject
{
	GENERATED_BODY()
	
public:
	UProjectileActorBuilder(const FObjectInitializer& ObjectInitializer);
public:
	void InitBuilder(class AMyShootingGameGameMode* InGameMode);

	void TriggerCondition(class AActor* InTriggingActor);

public:
	void LoadAnimFile(FString InFileRefPath);

protected:
	bool _CheckCondition(class AActor* InTriggingActor, const TArray<FConditionProjectile>& InCondList);
	void _Exec(class AActor* InTriggingActor, const TArray< FExecuteProjectile>& InExecList);


protected:
	UPROPERTY()
	TArray<FProjectileAnim>				mAnimList;

	UPROPERTY()
	class AMyShootingGameGameMode* mGameMode;
};
