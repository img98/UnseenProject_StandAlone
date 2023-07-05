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


void AFlamethrowerTurret::Fire() //�����ٹ���~
{
	bCanFire = false;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	//��� ���⼭ Pawn���� �ϸ� ��ĳ���͵� ������ �����Ŷ� ���ο� ä�����ִ°� �±��ѵ�, ȭ�����Ⱑ �ʹ� op�� �̷����Ƽ�� ������ ������
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
	//��� �ٴ���Ʈ�ȴ�. �����⤷�ߴ��... ������ �Ƹ� �� �ڵ���ü�� ������ ȣ��Ǽ� �׷���? ���� �������� ��¦�� �ٸ������..
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
		if (IsValid(HitEnemy[i].GetActor())) //�̺κп��� �迭���������� ũ������
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

	//TODO:: IsPower�� ���������� ������ �ҵ����°� ���� SetMaterialindex�� GunMesh �ٲٸ��
}

void AFlamethrowerTurret::TurretBehaviorStateMachine(float DeltaTime) //SetTurretActivation�Լ��� �־�����ؼ� override�� �����ʿ��־���
{

	switch (TurretState)
		{
		case ETurretState::ETS_OnBuild:
		{
			FHitResult CurorHitResult;
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
			SetTurretActivation(true); //��׸� �ٸ�!

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