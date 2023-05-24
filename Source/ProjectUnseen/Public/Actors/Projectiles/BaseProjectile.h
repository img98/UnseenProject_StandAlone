// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class ABaseTurret;

UCLASS()
class PROJECTUNSEEN_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
		
public:	
	ABaseProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "CustomClassSetup")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "CustomClassSetup")
	UParticleSystem* HitParticle;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	ABaseTurret* ParentTurret;

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetParnetTurret(ABaseTurret* InParentTurret) { ParentTurret = InParentTurret; }
};
