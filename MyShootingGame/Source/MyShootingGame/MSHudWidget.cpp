// Fill out your copyright notice in the Description page of Project Settings.


#include "MSHudWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

void UMSHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mTextCountArr.Add(CastChecked<UTextBlock>(GetWidgetFromName(TEXT("TextBlockA1Count"))));
	mTextCountArr.Add(CastChecked<UTextBlock>(GetWidgetFromName(TEXT("TextBlockA2Count"))));
	mTextCountArr.Add(CastChecked<UTextBlock>(GetWidgetFromName(TEXT("TextBlockA3Count"))));
	mTextCountArr.Add(CastChecked<UTextBlock>(GetWidgetFromName(TEXT("TextBlockA4Count"))));

	mButtonReset = CastChecked<UButton>(GetWidgetFromName(TEXT("ButtonReset")));
	mButtonReset->OnClicked.AddDynamic(this, &UMSHudWidget::OnClickedReset);

	mProgressBarCharging = CastChecked<UProgressBar>(GetWidgetFromName(TEXT("ProgressBarCharging")));
	mProgressBarCharging->SetVisibility(ESlateVisibility::Collapsed);

	mProjCount.Add(0);
	mProjCount.Add(0);
	mProjCount.Add(0);
	mProjCount.Add(0);

	_OnResetCount();
	mChargingMax = 0.f;
	mChargingDelta = 0.f;
	mIsShowCharging = false;
}

void UMSHudWidget::NativeDestruct()
{
	Super::NativeDestruct();

	mButtonReset->OnClicked.RemoveAll(this);
}

void UMSHudWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (mIsShowCharging)
	{
		mChargingDelta += InDeltaTime;
		float rate = mChargingDelta / mChargingMax;
		mProgressBarCharging->SetPercent(FMath::Min(rate, 1.0f));
	}
}

void UMSHudWidget::IncreaseProjCount(int32 InIndex, int32 InCount)
{
	if (!mProjCount.IsValidIndex(InIndex)) return;

	mProjCount[InIndex] += InCount;
	mTextCountArr[InIndex]->SetText(FText::FromString(FString::FormatAsNumber(mProjCount[InIndex])));
}

void UMSHudWidget::IncreaseCharging(float InMax)
{
	if (mIsShowCharging) return;

	mIsShowCharging = true;
	mChargingMax = InMax;
	mChargingDelta = 0;

	mProgressBarCharging->SetPercent(0.f);

	if (!mProgressBarCharging->IsVisible())
		mProgressBarCharging->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMSHudWidget::HideCharging()
{
	mIsShowCharging = false;
	mChargingDelta = 0.f;
	mProgressBarCharging->SetVisibility(ESlateVisibility::Collapsed);
}

void UMSHudWidget::OnClickedReset()
{
	_OnResetCount();
}

void UMSHudWidget::_OnResetCount()
{
	const int32 ProjCount = mProjCount.Num();
	for (int32 i = 0; i < ProjCount; ++i)
		mProjCount[i] = 0;

	for (int32 i = 0; i < ProjCount; ++i)
		mTextCountArr[i]->SetText(FText::FromString(FString::FormatAsNumber(mProjCount[i])));
}
