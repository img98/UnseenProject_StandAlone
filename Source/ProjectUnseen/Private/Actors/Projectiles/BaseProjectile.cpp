// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(SphereCollision);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	SphereCollision->SetSphereRadius(4.f);
	SphereCollision->SetCollisionProfileName(TEXT("Projectile"));
	/** Physics�� advanced�� WalkableSlope & Collision�� CanCharacterStepUpOn�� �ٲ���� �ϴ°�?*/
	//SphereCollision->GetWalkableSlopeOverride ???

	BulletMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	BulletMesh->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.3f));
	BulletMesh->SetGenerateOverlapEvents(false);
	/** Physics-AutoWeld �Ȳ��൵ �ݸ��������� NoCollision���� �ϸ� ������ ������?*/
	BulletMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BulletMesh->SetCastShadow(false);
	ProjectileMovementComponent->InitialSpeed = 30000.f;
	ProjectileMovementComponent->MaxSpeed = 30000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	//velocity(�����̼Ǹ��� ���״�� velocity���� �ִ���)�� bounce������ �ʿ��Ѱ�?
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Velocity = FVector(300.f, 0.f, 0.f);

	SphereCollision->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit); //Overlap�� �ƴ϶� OnHit�� OnComponentHit ���
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location, 1.5f, 1.5f); //voulume&pitch multiplier = 0.5, 1.5
	if (!IsValid(OtherActor))
	{
		Destroy();
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location);
		UE_LOG(LogTemp, Warning, TEXT("Projectile On Hit!"));
		Destroy();
	}

}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

