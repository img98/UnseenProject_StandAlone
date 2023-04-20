// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuildingActor.generated.h"

class UStatComponent;

UCLASS()
class PROJECTUNSEEN_API ABaseBuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBuildingActor();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "CustomClassSetup")
	UStatComponent* StatComponent;

public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UStatComponent* GetStatComponent() { return StatComponent; }
};
