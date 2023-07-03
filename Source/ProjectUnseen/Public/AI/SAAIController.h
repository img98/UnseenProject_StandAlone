// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUNSEEN_API ASAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASAAIController(const FObjectInitializer& ObjectInitializer);

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
