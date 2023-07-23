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


void AFlamethrowerTurret::Fire() //Multi Hit�� �������� �ʴ´�. -> �Ǹ����� �ް� GetOverlappingActors�� �Ẹ��.
{
	bCanFire = false;

	//�����ؼ� 15�� �̳��� ������ �߻����� �ʴ´�.
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
	AttackBox->GetOverlappingActors(OverlappedArray, AEnemyCharacter::StaticClass()); //EnemyCharacter�� �״�� �����ð� �ƴ϶� �ٸ� ������� ������?
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

	//TODO:: IsPower�� ���������� ������ �ҵ����°� ���� SetMaterialindex�� GunMesh �ٲٸ��
}

void AFlamethrowerTurret::TurretBehaviorStateMachine(float DeltaTime) //SetTurretActivation�Լ��� �־�����ؼ� override�� �����ʿ��־���
{

	switch (TurretState)
		{
		case ETurretState::ETS_OnBuild:
		{
			FHitResult CurorHitResult;
			//Ŀ����ġ�� �̵��Ҷ��� tick���ư���?
			UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, CurorHitResult); // �Ŀ� Build�� traceChannel���� �ٲܰ�, traceComplex�������� true

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