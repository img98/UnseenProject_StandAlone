// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "TeslaTurret.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTUNSEEN_API ATeslaTurret : public ABaseTurret
{
	GENERATED_BODY()

public:
	ATeslaTurret();

	virtual void Fire() override;

	virtual void Tick(float DeltaTime) override;

	void GetGunFever(float Fever);

	void CoolingGunFever(float DeltaTime);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HotMetal;
	UPROPERTY()
	float HotMetal_Intensity;
	UPROPERTY()
	float GunFever = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileSpawner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	UNiagaraComponent* TeslaFx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	UParticleSystem* HitParticle;

	//No TeslaBeam Particle

private:

};
