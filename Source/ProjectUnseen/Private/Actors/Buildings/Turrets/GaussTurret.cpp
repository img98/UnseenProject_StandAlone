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

	HotMetal->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StatComponent->SetStats(10.f, 10.f, 2.5f, 0.f);
}

void AGaussTurret::Fire()
{
	bCanFire = false;
	
	GetGunFever(GunFever);

	if (ProjectileClass && FireSound && MuzzleParticle)
	{
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
	HotMetal_Intensity = UKismetMathLibrary::FInterpTo(
		HotMetal_Intensity,
		0.f,
		DeltaTime,
		2.f);
	HotMetal->SetScalarParameterValueOnMaterials(FName(TEXT("Intensity")), HotMetal_Intensity);
}

void AGaussTurret::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BuildCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ChangeMeshMaterialToRed(TurretRootMesh);
	ChangeMeshMaterialToRed(TurretBodyMesh);
	ChangeMeshMaterialToRed(TurretGunMesh);

	ChangeMeshMaterialToRed(HotMetal);
}

void AGaussTurret::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::BuildCollisionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	ChangeMeshMaterialToGreen(TurretRootMesh);
	ChangeMeshMaterialToGreen(TurretBodyMesh);
	ChangeMeshMaterialToGreen(TurretGunMesh);

	ChangeMeshMaterialToGreen(HotMetal);
	
}

void AGaussTurret::BeginPlay()
{
	Super::BeginPlay();

	BuildCollision->OnComponentBeginOverlap.AddDynamic(this, &AGaussTurret::BuildCollisionBeginOverlap); //constructor에 넣으면 작동안하더라
	BuildCollision->OnComponentEndOverlap.AddDynamic(this, &AGaussTurret::BuildCollisionEndOverlap);
}

