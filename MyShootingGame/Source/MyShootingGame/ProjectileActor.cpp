// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "MyShootingGameGameMode.h"
#include "ProjectileActorBuilder.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom attached to the root (capsule)
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	ArrowComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// 1초로 제한 (속도 단위가 초)

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleComponent->SetupAttachment(ArrowComponent);
	CapsuleComponent->SetRelativeRotation(FVector(0, 0, 90).Rotation());
	CapsuleComponent->SetRelativeLocation(FVector(40, 0, 0));
	CapsuleComponent->SetCapsuleHalfHeight(42.f);
	CapsuleComponent->SetCapsuleRadius(8.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnHit);
	mWaitDestroy = false;
	mDestroyCauseBlock = false;
	mCheckDestroy = false;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mParam.mSpeed <= 0) 
		return;

	if (mParam.mLifeTime <= 0)
	{
		_OnDestroy();
		return;
	}

	mCheckDestroy = true;
	mParam.mLifeTime -= DeltaTime;
	FVector CurLoc = GetActorLocation();
	CurLoc += mParam.mDir * (mParam.mSpeed * DeltaTime);
	SetActorLocation(CurLoc);
}

void AProjectileActor::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AProjectileActor::StaticClass()))
	{
		if (OtherComp->IsA(UCapsuleComponent::StaticClass()))
		{
			mDestroyCauseBlock = true;
			_OnDestroy();
			return;
		}
	}
	else
	{
		mDestroyCauseBlock = true;
		_OnDestroy();
	}	
}

void AProjectileActor::_OnDestroy()
{
	if (!mCheckDestroy)
		return;

	mWaitDestroy = true;
	if (auto GameMode = Cast<AMyShootingGameGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		UProjectileActorBuilder* ActionBuilder = GameMode->GetActionBuilder();
		ActionBuilder->TriggerCondition(this);
	}

	Destroy();
}

void AProjectileActor::InitProjectile(const FProjectileParam& InParam)
{	
	mCheckDestroy = false;
	mParam = InParam;
	ArrowComponent->SetArrowColor(mParam.mColor.FromRGBE());
}
