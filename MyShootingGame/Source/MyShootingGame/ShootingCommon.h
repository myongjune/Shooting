#pragma once

#include "CoreMinimal.h"

//////////////////////////////////////////////////////////////////////////
// Value
//////////////////////////////////////////////////////////////////////////
struct FProjectileParam
{
public:
	float mLifeTime = 0;
	float mSpeed = 0;

	int mIndex = 0;

	FVector mDir;
	FVector mInitLoc;
	FVector mInitScale3D;

	FColor mColor = FColor(255, 0, 0);
};

