// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "Components/StatComponent.h"

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
	/** Physics의 advanced의 WalkableSlope & Collision의 CanCharacterStepUpOn을 바꿔줘야 하는가?*/
	//SphereCollision->GetWalkableSlopeOverride ???

	BulletMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	BulletMesh->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.3f));
	BulletMesh->SetGenerateOverlapEvents(false);
	/** Physics-AutoWeld 안꺼줘도 콜리젼프리셋 NoCollision으로 하면 괜찮지 않을까?*/
	BulletMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BulletMesh->SetCastShadow(false);
	ProjectileMovementComponent->InitialSpeed = 6000.f;
	ProjectileMovementComponent->MaxSpeed = 6000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	//velocity(로테이션말고 말그대로 velocity란이 있더라)와 bounce설정이 필요한가?
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Velocity = FVector(600.f, 0.f, 0.f);

	SphereCollision->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit); //Overlap이 아니라 OnHit라서 OnComponentHit 사용
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
		FDamageEvent DamageEvent;
		if (IsValid(ParentTurret))
		{
			OtherActor->TakeDamage(
				ParentTurret->GetStatComponent()->GetAttackDamage(),
				DamageEvent,
				UGameplayStatics::GetPlayerController(this, 0), //후에 ParentTurret의 ParentPlayer를 찾아서 컨트롤러를 연결해주자
				this //그저 this가 아니라 ParentTurret의 ParentPlayer로 해주는게 좋을지도 모르겠다.
			);
		}
		
		Destroy();
	}

}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

