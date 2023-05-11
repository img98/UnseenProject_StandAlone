// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/TurretBuildState.h"
#include "BaseBuildingActor.generated.h"

class UStatComponent;
class UBoxComponent;

UCLASS()
class PROJECTUNSEEN_API ABaseBuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBuildingActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void BindBuildCollisionOverlap(UObject& InClass);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BuildCollision;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* GreenMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* RedMaterial;

	UPROPERTY(VisibleAnywhere)
	UStatComponent* StatComponent;

	UPROPERTY(VisibleAnywhere)
	ETurretBuildState CurrentBuildState;




	UFUNCTION()
	virtual void BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void BuildCompleted();
	//마테리얼 색바꾸는 함수를 구현해야 한다.
	UFUNCTION(BlueprintCallable)
	virtual void ChangeMeshMaterialToGreen(UStaticMeshComponent* InMesh);
	UFUNCTION(BlueprintCallable)
	virtual void ChangeMeshMaterialToRed(UStaticMeshComponent* InMesh);

public:	
	FORCEINLINE UStatComponent* GetStatComponent() { return StatComponent; }
};
