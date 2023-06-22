// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/SAEnemySpawnPool.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "AI/SAAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASAEnemySpawnPool::ASAEnemySpawnPool()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnField = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnField"));
	SpawnField->SetupAttachment(RootComponent);
	SpawnField->SetGenerateOverlapEvents(false);
	SpawnField->SetCollisionProfileName(TEXT("NoCollision"));
}


void ASAEnemySpawnPool::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ASAEnemySpawnPool::Spawn,
		SpawnDelay,
		true //repeat Timer
	);
}

void ASAEnemySpawnPool::Spawn()
{
	// SpawnActor의 템플릿을 후에 DataAsset 테이블로 만들어, 생성되는 액터의 다양성을 줄 수 있겠다.
	// Spawn Parameter 사용X <-따로 필요성을 못느껴서
	AEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
		SpawnEnemyClass,
		UKismetMathLibrary::RandomPointInBoundingBox(SpawnField->GetComponentLocation(), SpawnField->GetScaledBoxExtent()),
		GetActorRotation()
	);
}

void ASAEnemySpawnPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

