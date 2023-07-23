// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/GaussTurret.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Projectiles/BaseProjectile.h"
#include "Components/BoxComponent.h"

AGaussTurret::AGaussTurret()
{
	HotMetal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HotMetal"));
	HotMetal->SetupAttachment(TurretGunMesh);
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(TurretGunMesh);

	//HotMetal->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AGaussTurret::Fire()
{
	bCanFire = false;
	GetGunFever(GunFever);

	if (ProjectileClass && FireSound && MuzzleParticle)
	{
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = InstigatorPawn;

		ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(
			ProjectileClass,
			ProjectileSpawner->GetComponentLocation(),
			ProjectileSpawner->GetComponentRotation(),
			SpawnParams
		);
		SpawnedProjectile->SetParnetTurret(this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ProjectileSpawner->GetComponentLocation());
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleParticle,
			ProjectileSpawner
		);
	}
}

void AGaussTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CoolingGunFever(DeltaTime);
}

void AGaussTurret::GetGunFever(float Fever)
{
	HotMetal_Intensity = Fever;
}
void AGaussTurret::CoolingGunFever(float DeltaTime)
{
	HotMetal_Intensity = UKismetMathLibrary::FInterpTo(//constant를 사용하니 불이 너무 확꺼지더라
		HotMetal_Intensity,
		0.f,
		DeltaTime,
		CoolingInterpSpeed);
	HotMetal->SetScalarParameterValueOnMaterials(FName(TEXT("Intensity")), HotMetal_Intensity);
}

void AGaussTurret::BeginPlay()
{
	Super::BeginPlay();
}

