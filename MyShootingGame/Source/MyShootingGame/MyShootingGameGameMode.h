// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingCommon.h"
#include "MyShootingGameGameMode.generated.h"

UCLASS(minimalapi)
class AMyShootingGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UProjectileActorBuilder* mActionBuilder;

	UPROPERTY()
	class UMSHudWidget*		mHudWidget;

protected:
	bool		mIsTouchedFire;
	bool		mIsReleasedFire;

	bool		mIsTouchedSpecialFire;
	bool		mIsReleassedSpecialFire;

	float		mWhenTouchedFireTimeSec;


public:
	virtual void BeginPlay() override;

public:
	// Fire ��ư ������ ��
	void TouchFireStart();

	// Fire ��ư Release
	void TouchFireRelease();

	// Special Fire ��ư ������ ��
	void TouchSpecialFireStart();

	// Specail Fire ��ư Release
	void TouchSpecialFireRelease();


public:
	void SpawnProjectile(FString InPath, const FProjectileParam& InParam);	
	void IncreaseProjCount(int32 InIndex);
	void IncreaseCharging(float InMax);
	void HideCharging();

public:
	bool IsFirePressed() const				{ return mIsTouchedFire; }
	bool IsFireReleased() const			{ return mIsReleasedFire;	}

	bool IsSpecialFirePressed() const	{ return mIsTouchedSpecialFire;	}
	bool IsSpecialFireReleased() const	{ return mIsReleassedSpecialFire; }


	float GetFirePressedTime() const;

public:
	class UProjectileActorBuilder* GetActionBuilder() {	return mActionBuilder; }

public:
	AMyShootingGameGameMode();
};



