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
#include "Components/BoxComponent.h"

AFlamethrowerTurret::AFlamethrowerTurret()
{
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(TurretGunMesh);
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(ProjectileSpawner);
	LoopingFiresound = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopingFiresound"));
	LoopingFiresound->SetupAttachment(TurretGunMesh);
	LoopingFiresound->SetAutoActivate(false);
	PersistentFireFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PersistentFireFX"));
	PersistentFireFX->SetupAttachment(ProjectileSpawner);
	PersistentFireFX->SetAutoActivate(false);
}


void AFlamethrowerTurret::Fire()
{
	bCanFire = false;
	if (CheckDotproduct()) return;

	SetTurretActivation(true);
	TArray<AActor*> OverlappedArray;
	AttackBox->GetOverlappingActors(OverlappedArray, AEnemyCharacter::StaticClass()); //EnemyCharacter를 그대로 가져올게 아니라 다른 참조방법 없을까?
	for (auto OverlappedActors : OverlappedArray)
	{
		FDamageEvent DamageEvent;
		OverlappedActors->TakeDamage(
			GetStatComponent()->GetAttackDamage(),
			DamageEvent,
			UGameplayStatics::GetPlayerController(this, 0),
			this);
	}

}

void AFlamethrowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFlamethrowerTurret::TurretBehaviorStateMachine(float DeltaTime) //SetTurretActivation함수를 넣어줘야해서 override로 만들필요있었음
{

	switch (TurretState)
		{
		case ETurretState::ETS_OnBuild:
		{
			FHitResult CurorHitResult;
			//커서위치가 이동할때만 tick돌아가게?
			UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, CurorHitResult); // 후에 Build용 traceChannel만들어서 바꿀것, traceComplex먼지몰라서 true

			float InGridSize = 50.f;
			FVector BuildPosition = UKismetMathLibrary::Vector_SnappedToGrid(CurorHitResult.Location, InGridSize);

			SetActorLocation(BuildPosition);
			break;
		}
		case ETurretState::ETS_Searching:
		{
			if (EnemyArray.Num() > 0)
			{
				SetTurretState(ETurretState::ETS_InCombat);
			}
			break;
		}
		case ETurretState::ETS_InCombat:
		{
			LookAtEnemy(DeltaTime);

			if (EnemyArray.Num() < 1)
			{
				SetTurretActivation(false);
				SetTurretState(ETurretState::ETS_Searching);
			}
			else if (bCanFire)
			{
				Fire();
				FireDelay();
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