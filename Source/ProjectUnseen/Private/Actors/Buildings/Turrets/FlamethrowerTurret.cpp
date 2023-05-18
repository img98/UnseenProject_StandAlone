// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/FlamethrowerTurret.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

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

	StatComponent->SetStats(10.f, 1.f, 0.1f, 0.f);
}


void AFlamethrowerTurret::Fire()
{
	bCanFire = false;
	//UKismetSystemLibrary::SphereTraceMultiForObjects(this,start, end, radiius, )
	/** 
	UKismetSystemLibrary::SphereTraceMulti(
		this,
		ProjectileSpawner->GetComponentLocation(),
		DamageRadius * ProjectileSpawner->GetForwardVector() + ProjectileSpawner->GetComponentLocation(),
		SphereTraceRadius,

		)
		*/

	//아니 얘는 그냥 트레이스 쓰지말고, vfx위치에다가 안보이는 캡슐실린더 달아주면 될거같은데?
}

void AFlamethrowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO:: IsPower가 들어와있을때 전구에 불들어오는것 구현 SetMaterialindex로 GunMesh 바꾸면됨
}

void AFlamethrowerTurret::TurretBehaviorStateMachine(float DeltaTime)
{

	switch (TurretState)
	{
	case ETurretState::ETS_OnBuild:
	{
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
		SetTurretActivation(true);

		if (EnemyArray.Num() < 1)
		{
			SetTurretActivation(false);
			SetTurretState(ETurretState::ETS_Searching);
		}
		if (bCanFire)
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

void AFlamethrowerTurret::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AFlamethrowerTurret::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}