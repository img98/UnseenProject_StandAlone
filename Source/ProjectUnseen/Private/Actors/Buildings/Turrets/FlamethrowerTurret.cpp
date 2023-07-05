// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/FlamethrowerTurret.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameData/SAStatData.h"

AFlamethrowerTurret::AFlamethrowerTurret()
{
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(TurretGunMesh);
	LoopingFiresound = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopingFiresound"));
	LoopingFiresound->SetupAttachment(TurretGunMesh);
	LoopingFiresound->SetAutoActivate(false);
	PersistentFireFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PersistentFireFX"));
	PersistentFireFX->SetupAttachment(ProjectileSpawner);
	PersistentFireFX->SetAutoActivate(false);
}


void AFlamethrowerTurret::Fire() //문제다문제~
{
	bCanFire = false;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	//사실 여기서 Pawn으로 하면 내캐릭터도 데미지 받을거라 새로운 채널파주는게 맞긴한데, 화염방사기가 너무 op니 이런페널티도 나쁘지 않을듯
	ObjectType.Emplace(ECollisionChannel::ECC_Pawn);
	TArray<FHitResult> HitEnemy;
	/** 
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		ProjectileSpawner->GetComponentLocation(),
		DamageRadius * ProjectileSpawner->GetForwardVector() + ProjectileSpawner->GetComponentLocation(),
		22.f,
		ObjectType,
		true,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitEnemy,
		true
	);
	*/
	UKismetSystemLibrary::SphereTraceMultiByProfile(
		this,
		ProjectileSpawner->GetComponentLocation(),
		DamageRadius * ProjectileSpawner->GetForwardVector() + ProjectileSpawner->GetComponentLocation(),
		100.f,
		FName("EnemyCapsulePreset"),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitEnemy,
		true
	);
	//얘는 다단히트된다. 버그잡ㅇ야대ㅗ... 원인은 아마 이 코드자체가 여러번 호출되서 그런듯? 들어가는 데미지가 살짝씩 다르더라고..
	UE_LOG(LogTemp, Error, TEXT("!"));
	for (FHitResult HitResult : HitEnemy)
	{
		FString HitResultName = HitResult.GetActor()->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s"), *HitResultName);
	}
	/** 
	for (FHitResult HitResult : HitEnemy)
	{
		FDamageEvent DamageEvent;
		if (IsValid(HitResult.GetActor()))
		{
			FString HitResultName = HitResult.GetActor()->GetName();
			UE_LOG(LogTemp, Error, TEXT("%s"),*HitResultName);
			HitResult.GetActor()->TakeDamage(
				GetStatComponent()->GetAttackDamage(),
				DamageEvent,
				UGameplayStatics::GetPlayerController(this, 0),
				this
			);
		}
	}
	*/
	/** 
	for (auto i = 0; HitEnemy.Num(); ++i)
	{
		FDamageEvent DamageEvent;
		if (IsValid(HitEnemy[i].GetActor())) //이부분에서 배열참조과정에 크러쉬남
		{
			HitEnemy[i].GetActor()->TakeDamage(
				GetStatComponent()->GetAttackDamage(),
				DamageEvent,
				UGameplayStatics::GetPlayerController(this, 0),
				this
			);
		}
	}
	*/
}

void AFlamethrowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO:: IsPower가 들어와있을때 전구에 불들어오는것 구현 SetMaterialindex로 GunMesh 바꾸면됨
}

void AFlamethrowerTurret::TurretBehaviorStateMachine(float DeltaTime) //SetTurretActivation함수를 넣어줘야해서 override로 만들필요있었음
{

	switch (TurretState)
		{
		case ETurretState::ETS_OnBuild:
		{
			FHitResult CurorHitResult;
			UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, CurorHitResult); // 후에 Build용 traceChannel만들어서 바꿀것, traceComplex먼지몰라서 true

			float InGridSize = 50.f;
			FVector BuildPosition = UKismetMathLibrary::Vector_SnappedToGrid(CurorHitResult.Location, InGridSize);

			SetActorLocation(BuildPosition);
			break;
		}
		case ETurretState::ETS_Searching:
		{
			RotateTurret();
			if (EnemyArray.Num() > 0)
			{
				SetTurretState(ETurretState::ETS_InCombat);
			}
			break;
		}
		case ETurretState::ETS_InCombat:
		{
			LookAtEnemy(DeltaTime);
			SetTurretActivation(true); //얘네만 다름!

			if (EnemyArray.Num() < 1)
			{
				SetTurretActivation(false);
				SetTurretState(ETurretState::ETS_Searching);
			}
			else if (bCanFire)
			{
				Fire();
				FireDelay(DeltaTime);
			}
			break;
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] TurretState is Default!"), *GetName());
			break;
		}
	}
}

void AFlamethrowerTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AFlamethrowerTurret::SetTurretActivation(bool bBool)
{
	if (bBool)
	{
		PersistentFireFX->Activate();
		LoopingFiresound->Activate();
	}
	else
	{
		PersistentFireFX->Deactivate();
		LoopingFiresound->Deactivate();
	}
}