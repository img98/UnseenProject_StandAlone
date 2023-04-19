// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTUNSEEN_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
		
public:	
	ABaseProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = ClassSetup)
	USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, Category = ClassSetup)
	UStaticMeshComponent* BulletMesh;
	UPROPERTY(VisibleAnywhere, Category = ClassSetup)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = ClassSetup)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = ClassSetup)
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Category = ClassSetup)
	float DamageValue=0;

public:	
	virtual void Tick(float DeltaTime) override;

};
