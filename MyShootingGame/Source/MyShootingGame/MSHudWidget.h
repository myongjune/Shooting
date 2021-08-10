// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTINGGAME_API UMSHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void IncreaseProjCount(int32 InIndex, int32 InCount);
	void IncreaseCharging(float InMax);

	void HideCharging();

public:
	UFUNCTION()
	void OnClickedReset();

protected:
	void _OnResetCount();
	
protected:
	UPROPERTY()
	TArray<class UTextBlock*>	mTextCountArr;

	UPROPERTY()
	class  UButton*		mButtonReset;

	UPROPERTY()
	class UProgressBar* mProgressBarCharging;

protected:
	TArray<int>		mProjCount;
	float					mChargingMax;
	float					mChargingDelta;
	bool					mIsShowCharging;
};
