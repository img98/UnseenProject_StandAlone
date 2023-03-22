// Fill out your copyright notice in the Description page of Project Settings.


#include "SAGameMode.h"
#include "Characters/Player/PlayerCharacter.h"


ASAGameMode::ASAGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

void ASAGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("PostLogin End"));
}
