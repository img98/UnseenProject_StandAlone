// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "GaussTurret.generated.h"

UCLASS()
class PROJECTUNSEEN_API AGaussTurret : public ABaseTurret
{
	GENERATED_BODY()
	
public:
	AGaussTurret();

	virtual void Fire() override;

	virtual void Tick(float DeltaTime) override;

	void GetGunFever(float Fever);

	void CoolingGunFever(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Body")
	USceneComponent* ProjectileSpawner;
	UPROPERTY(VisibleAnywhere, Category = "Body")
	UStaticMeshComponent* HotMetal;
	UPROPERTY()
	float HotMetal_Intensity;
	UPROPERTY()
	float GunFever=6.f;
	
	float CoolingInterpSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	TSubclassOf<ABaseProjectile> ProjectileClass;

private:

};
