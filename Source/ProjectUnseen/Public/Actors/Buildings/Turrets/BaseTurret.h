// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/BaseBuildingActor.h"
#include "Enum/TurretState.h"
#include "TurretInterface.h"

#include "BaseTurret.generated.h"

class USphereComponent;
class AEnemyCharacter;

UCLASS()
class PROJECTUNSEEN_API ABaseTurret : public ABaseBuildingActor, public ITurretInterface
{

	GENERATED_BODY()
	
public:
	ABaseTurret();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	ETurretState TurretState = ETurretState::ETS_MAX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretRootMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretBodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretGunMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* FireField;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AEnemyCharacter*> EnemyArray;

	UFUNCTION()
	FORCEINLINE void SetTurretState(ETurretState InState) { TurretState = InState; };

	UFUNCTION()
	void RotateTurret();

	UFUNCTION()
	virtual void FireFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void FireFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float FireSpeed;

private:

};
