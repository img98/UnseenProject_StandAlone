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


void AFlamethrowerTurret::Fire() //Multi Hit가 고쳐지질 않는다. -> 실린더를 달고 GetOverlappingActors를 써보자.
{
	bCanFire = false;

	//내적해서 15도 이내에 없으면 발사하지 않는다.
	AEnemyCharacter* Target = EnemyArray[0].Get();
	float DotProduct = FVector::DotProduct(RotateGunAnchor->GetForwardVector(), Target->GetActorLocation() - this->GetActorLocation());
	float AcosAngle = FMath::Acos(DotProduct);
	float DotProductAngle = FMath::RadiansToDegrees(AcosAngle);
	if (DotProductAngle / 2 > 15.f)
	{
		return;
	}

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

	//TODO:: IsPower가 들어와있을때 전구에 불들어오는것 구현 SetMaterialindex로 GunMesh 바꾸면됨
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