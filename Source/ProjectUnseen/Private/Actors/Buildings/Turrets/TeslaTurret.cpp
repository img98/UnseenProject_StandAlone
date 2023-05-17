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

	StatComponent->SetStats(10.f, 10.f, 2.5f, 0.f);
}

void ATeslaTurret::Fire()
{
	bCanFire = false;

	GetGunFever(GunFever);

	if (FireSound && MuzzleParticle)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ProjectileSpawner->GetComponentLocation());
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleParticle,
			ProjectileSpawner
		);
	}

	// No Projectile -> Approach by Memory
	if (EnemyArray.IsValidIndex(0))
	{
		AEnemyCharacter* Target = EnemyArray[0];
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
		2.f);
	HotMetal->SetScalarParameterValueOnMaterials(FName(TEXT("Intensity")), HotMetal_Intensity);
}

void ATeslaTurret::BeginPlay()
{
	Super::BeginPlay();
}

void ATeslaTurret::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATeslaTurret::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ATeslaTurret::BuildCompleted()
{
}
