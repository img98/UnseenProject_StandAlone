// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SAPlayerController.h"

void ASAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Error, TEXT("post initialize"));
}

void ASAPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Error, TEXT("OnPossess"));
}
