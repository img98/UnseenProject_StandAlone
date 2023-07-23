// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/GaussTurret.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Projectiles/BaseProjectile.h"
#include "Components/BoxComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"

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
	
	//내적해서 15도 이내에 없으면 발사하지 않는다.
	AEnemyCharacter* Target = EnemyArray[0].Get();
	float DotProduct = FVector::DotProduct(RotateGunAnchor->GetForwardVector(), Target->GetActorLocation() - this->GetActorLocation());
	float AcosAngle = FMath::Acos(DotProduct);
	float DotProductAngle = FMath::RadiansToDegrees(AcosAngle);
	if (DotProductAngle / 2 > 15.f)
	{
		return;
	}

	// Spawn Projectile and Play Sound, Spawn Emitter
	check(ProjectileClass && FireSound && MuzzleParticle)
	GetGunFever(GunFever);
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

