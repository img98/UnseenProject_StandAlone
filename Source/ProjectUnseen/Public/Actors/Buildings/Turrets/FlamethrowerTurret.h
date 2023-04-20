// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "FlamethrowerTurret.generated.h"

class UParticleSystemComponent;

UCLASS()
class PROJECTUNSEEN_API AFlamethrowerTurret : public ABaseTurret
{
	GENERATED_BODY()

public:
	AFlamethrowerTurret();

	virtual void Fire() override;

	virtual void Tick(float DeltaTime) override;

	virtual void TurretBehaviorStateMachine(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void SetTurretActivation(bool bBool);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileSpawner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* LoopingFiresound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* PersistentFireFX;

	UPROPERTY()
	float DamageRadius = 800.f;
	float SphereTraceRadius = 20.f;


	//No TeslaBeam Particle

private:

};