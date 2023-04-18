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
	void OnH	it(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:	
	virtual void Tick(float DeltaTime) override;

};
