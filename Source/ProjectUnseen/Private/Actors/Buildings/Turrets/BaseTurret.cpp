﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "Components/SphereComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Projectiles/BaseProjectile.h"
#include "Components/StatComponent.h"


ABaseTurret::ABaseTurret()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	TurretRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRootMesh"));
	TurretRootMesh->SetupAttachment(Root);
	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBodyMesh"));
	TurretBodyMesh->SetupAttachment(TurretRootMesh);
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGunMesh"));
	TurretGunMesh->SetupAttachment(TurretBodyMesh);
	RotateGunAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RotateGunAnchor"));
	RotateGunAnchor->SetupAttachment(TurretGunMesh);
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(TurretGunMesh);
	FireField = CreateDefaultSubobject<USphereComponent>(TEXT("FireField"));
	FireField->SetupAttachment(GetRootComponent());
	
	TurretRootMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TurretBodyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TurretGunMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TurretState = ETurretState::ETS_Searching;
}

void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (TurretState)
	{
	case ETurretState::ETS_NonCombat:
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
			if (EnemyArray.Num() < 1)
			{
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

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	FireField->OnComponentBeginOverlap.AddDynamic(this, &ABaseTurret::FireFieldBeginOverlap);
	FireField->OnComponentEndOverlap.AddDynamic(this, &ABaseTurret::FireFieldEndOverlap);

}

void ABaseTurret::RotateTurret()
{
	// TODO: 의미없이 빙빙돌기 구현하기 or 고개숙이고 대기하기 구현
}

void ABaseTurret::LookAtEnemy(float DeltaTIme)
{
	if (!EnemyArray.IsValidIndex(0)) return; //이걸 안해주니까 시작하자마자 크래쉬나더라

	AEnemyCharacter* Target = EnemyArray[0];
	if (Target == nullptr) return;

	const FRotator LookTargetRotator = UKismetMathLibrary::FindLookAtRotation(RotateGunAnchor->GetComponentLocation(), Target->GetActorLocation());
	const FRotator InterpTargetRotator = UKismetMathLibrary::RInterpTo(RotateGunAnchor->GetComponentRotation(), LookTargetRotator, DeltaTIme, RotateInterpSpeed);
	TurretBodyMesh->SetWorldRotation(FRotator(0.f, InterpTargetRotator.Yaw, 0.f));
	TurretGunMesh->SetRelativeRotation(FRotator(InterpTargetRotator.Pitch, 0.f, 0.f));

	// TODO : Rotate가 현위치에서 목적지까지 부드럽게 움직이는 방법을 고안해보자.

}

void ABaseTurret::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Fire함수의 재정의가 이루어지지 않았습니다. 자식Class에서 정의해주세요"), *GetName());

	bCanFire = false;
	if (ProjectileClass && FireSound && MuzzleParticle)
	{
		// 씬컴포넌트로해서 따로 트랜스폼 안따줘도 될듯? //FTransform ProjectileSpawnTransform = ProjectileSpawner->GetComponentTransform();
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());
		FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = TODO ; 
		SpawnParams.Instigator = InstigatorPawn;

		ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(
			ProjectileClass,
			ProjectileSpawner->GetComponentLocation(),
			ProjectileSpawner->GetComponentRotation(),
			SpawnParams
		);
		SpawnedProjectile->SetParnetTurret(this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ProjectileSpawner->GetComponentLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, ProjectileSpawner->GetComponentLocation());
	}
}

void ABaseTurret::FireDelay(float DeltaTime)
{
	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		FTimerDelegate::CreateLambda([&]() {
			//TODO : Just Wait for 'Firespeed' seconds & ClearTimer
			UE_LOG(LogTemp, Warning, TEXT("Fire!"));
			bCanFire = true;
			GetWorld()->GetTimerManager().ClearTimer(FireTimer);
			}),
		StatComponent->GetAttackSpeed(),
		false
	);
}

void ABaseTurret::FireFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Emplace(EnemyCharacter);
}

void ABaseTurret::FireFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Remove(EnemyCharacter);
}
