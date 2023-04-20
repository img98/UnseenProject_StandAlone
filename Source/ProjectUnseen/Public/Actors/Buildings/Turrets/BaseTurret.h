// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/BaseBuildingActor.h"
#include "Enum/TurretState.h"
#include "TurretInterface.h"

#include "BaseTurret.generated.h"

class USphereComponent;
class AEnemyCharacter;
class ABaseProjectile;

UCLASS()
class PROJECTUNSEEN_API ABaseTurret : public ABaseBuildingActor, public ITurretInterface
{

	GENERATED_BODY()
	
public:
	ABaseTurret();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// �ͷ� ������Ʈ ����
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	UStaticMeshComponent* TurretRootMesh;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	UStaticMeshComponent* TurretBodyMesh;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	UStaticMeshComponent* TurretGunMesh;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	USceneComponent* RotateGunAnchor;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	USceneComponent* ProjectileSpawner;
	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	USphereComponent* FireField;

	//�ͷ� �� projectile ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	UParticleSystem* MuzzleParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	USoundBase* FireSound;

	//�ͷ� ������Ƽ
	FTimerHandle FireTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETurretState TurretState = ETurretState::ETS_MAX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AEnemyCharacter*> EnemyArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateInterpSpeed = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanFire = true;

	UFUNCTION()
	FORCEINLINE void SetTurretState(ETurretState InState) { TurretState = InState; };

	UFUNCTION()
	void RotateTurret();
	UFUNCTION()
	void LookAtEnemy(float DeltaTime);

	UFUNCTION()
	virtual void FireFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void FireFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Fire();
	UFUNCTION()
	virtual void FireDelay(float DeltaTime);


private:

};
