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

void ASAGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start Initial Spawn
	GetWorld()->GetTimerManager().SetTimer(
		InitialSpawnTimer,
		this,
		&ASAGameMode::InitialSpawn,
		5.f,
		false
	);

	// Start Increase Pool Size
	GetWorld()->GetTimerManager().SetTimer(
		IncreasePoolSizeTimer,
		this,
		&ASAGameMode::IncreasePoolSize,
		10.f, //DataAsset으로 관리하는게 나을지도?
		true
	);
}


void ASAGameMode::InitialSpawn()
{
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] InitialSpawn Broadcasted!"));
	OnInitialSpawn.Broadcast();
}

void ASAGameMode::IncreasePoolSize()
{
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] IncreasePoolSize Broadcasted!"));
	OnIncreasePoolSize.Broadcast();
}
