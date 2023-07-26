// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/SAEnemySpawnPool.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "AI/SAAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SAGameMode.h"

ASAEnemySpawnPool::ASAEnemySpawnPool()
{
	PrimaryActorTick.bCanEverTick = false; //tick�Ƚᵵ�ɵ�

	SpawnField = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnField"));
	SpawnField->SetupAttachment(RootComponent);
	SpawnField->SetGenerateOverlapEvents(false);
	SpawnField->SetCollisionProfileName(TEXT("NoCollision"));

}


void ASAEnemySpawnPool::BeginPlay()
{
	Super::BeginPlay();

	ASAGameMode* GameMode = Cast<ASAGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnInitialSpawn.AddUObject(this, &ASAEnemySpawnPool::Spawn);
	GameMode->OnIncreasePoolSize.AddUObject(this, &ASAEnemySpawnPool::IncreasePoolSize);

	//Fill SpawningPool
	for (int32 i = 0; i < MaxPoolPopulation; ++i)
	{
		// SpawnActor�� ���ø��� �Ŀ� DataAsset ���̺�� �����, �����Ǵ� ������ �پ缺�� �� �� �ְڴ�.
		AEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
			SpawnEnemyClass,
			UKismetMathLibrary::RandomPointInBoundingBox(SpawnField->GetComponentLocation(), SpawnField->GetScaledBoxExtent()),
			GetActorRotation()
		);
		SpawningPool.Emplace(SpawnedCharacter);
	}
}

void ASAEnemySpawnPool::Spawn() //Delegate�� ���� ����
{
	if (SpawningPool.IsEmpty() == false)
	{
		int32 ValidIndex = -1;
		for (int32 idx = 0; idx < SpawningPool.Num(); ++idx)
		{
			if (SpawningPool[idx]->GetActivated() == false)
			{
				ValidIndex = idx;
			}
		}
		if (ValidIndex != -1)
		{
			AEnemyCharacter* SpawningCharacter = SpawningPool[ValidIndex];
			SpawningCharacter->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(SpawnField->GetComponentLocation(), SpawnField->GetScaledBoxExtent()));
			SpawningCharacter->SetActivated(true);
		}
	}

	//SpawnDealy�ð��ڿ� �ٽ� ��������� �Լ�ȣ��
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ASAEnemySpawnPool::Spawn,
		SpawnDelay,
		false //repeat Timer, �̹� �Լ������� ��������� ȣ���ϱ⿡ ����� False
	);
}

void ASAEnemySpawnPool::IncreasePoolSize()
{
	for (int32 i = 0; i < IncreaseRate; ++i)
	{
		AEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
			SpawnEnemyClass,
			UKismetMathLibrary::RandomPointInBoundingBox(SpawnField->GetComponentLocation(), SpawnField->GetScaledBoxExtent()),
			GetActorRotation()
		);
		SpawningPool.Emplace(SpawnedCharacter);
		++MaxPoolPopulation;
	}
}

void ASAEnemySpawnPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

