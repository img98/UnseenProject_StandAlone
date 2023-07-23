// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/BaseBuildingActor.h"
#include "Enum/TurretState.h"
#include "TurretInterface.h"
#include "BaseTurret.generated.h"

UCLASS()
class PROJECTUNSEEN_API ABaseTurret : public ABaseBuildingActor, public ITurretInterface
{

	GENERATED_BODY()
	
public:
	ABaseTurret();

	virtual void Tick(float DeltaTime) override;
	virtual void TurretBehaviorStateMachine(float DeltaTime);
	virtual void BuildCompleted() override;

protected:
	virtual void BeginPlay() override;

	// �ͷ� ������Ʈ ����
	UPROPERTY(VisibleAnywhere, Category = "Body")
	UStaticMeshComponent* TurretRootMesh;
	UPROPERTY(VisibleAnywhere, Category = "Body")
	UStaticMeshComponent* TurretBodyMesh;
	UPROPERTY(VisibleAnywhere, Category = "Body")
	UStaticMeshComponent* TurretGunMesh;
	UPROPERTY(VisibleAnywhere, Category = "Body")
	USceneComponent* RotateGunAnchor;
	UPROPERTY(VisibleAnywhere, Category = "Body")
	class USphereComponent* FireField;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USAStatData* StatData;

	//�ͷ� �� projectile ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	UParticleSystem* MuzzleParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClassSetup")
	USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETurretState TurretState = ETurretState::ETS_MAX;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true")) //TWeakObjectPtr�� TArray�� ��ҷ� ���� ��� BP���� ���� �� ����. BlueprintReadWrite�� ���־��Ѵ�.
	TArray<TWeakObjectPtr<class AEnemyCharacter>> EnemyArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateInterpSpeed = 100.f;
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
	virtual void FireDelay();
	FTimerHandle FireTimer;


private:

};
