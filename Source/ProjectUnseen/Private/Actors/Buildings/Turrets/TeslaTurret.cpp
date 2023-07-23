// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/TeslaTurret.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"

ATeslaTurret::ATeslaTurret()
{
	HotMetal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HotMetal"));
	HotMetal->SetupAttachment(TurretGunMesh);
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(TurretGunMesh);
	TeslaFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeslaFx"));
	TeslaFx->SetupAttachment(TurretGunMesh);
}

void ATeslaTurret::Fire()
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

	check(FireSound && MuzzleParticle);
	GetGunFever(GunFever);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ProjectileSpawner->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAttached(
		MuzzleParticle,
		ProjectileSpawner
	);
	// No Projectile -> Approach by Memory
	if (EnemyArray.IsValidIndex(0))
	{
		if (Target && HitSound && HitParticle)
		{
			FDamageEvent DamageEvent;
			Target->TakeDamage(GetStatComponent()->GetAttackDamage(), DamageEvent, UGameplayStatics::GetPlayerController(this, 0), this);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Target->GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitParticle,
				Target->GetActorLocation(),
				ProjectileSpawner->GetComponentRotation(),
				FVector(0.4f) //Scale 0.4f will be fit
			);
		}
	}

}

void ATeslaTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CoolingGunFever(DeltaTime);

}

void ATeslaTurret::GetGunFever(float Fever)
{
	HotMetal_Intensity = Fever;
}
void ATeslaTurret::CoolingGunFever(float DeltaTime)
{
	HotMetal_Intensity = UKismetMathLibrary::FInterpTo(
		HotMetal_Intensity,
		0.f,
		DeltaTime,
		CoolingInterpSpeed);
	HotMetal->SetScalarParameterValueOnMaterials(FName(TEXT("Intensity")), HotMetal_Intensity);
}

void ATeslaTurret::BeginPlay()
{
	Super::BeginPlay();

}
