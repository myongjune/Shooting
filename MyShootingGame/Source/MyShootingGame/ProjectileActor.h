// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingCommon.h"
#include "ProjectileActor.generated.h"

UCLASS()
class MYSHOOTINGGAME_API AProjectileActor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

protected:
	FProjectileParam	mParam;
	bool					mWaitDestroy;
	bool					mDestroyCauseBlock;
	bool					mCheckDestroy;

public:
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void InitProjectile(const FProjectileParam& InParam);

public:
	FProjectileParam	GetProjectileParam() const { return mParam; }
	bool					IsWaitDestroy() const { return mWaitDestroy; }
	bool					IsDestroyCauseBlock() const { return mDestroyCauseBlock; }


protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	void _OnDestroy();
};
