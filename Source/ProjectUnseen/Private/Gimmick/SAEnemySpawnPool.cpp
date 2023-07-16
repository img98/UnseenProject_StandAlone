// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/SAEnemySpawnPool.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "AI/SAAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SAGameMode.h"

ASAEnemySpawnPool::ASAEnemySpawnPool()
{
	PrimaryActorTick.bCanEverTick = false; //tick안써도될듯

	SpawnField = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnField"));
	SpawnField->SetupAttachment(RootComponent);
	SpawnField->SetGenerateOverlapEvents(false);
	SpawnField->SetCollisionProfileName(TEXT("NoCollision"));

}


void ASAEnemySpawnPool::BeginPlay()
{
	Super::BeginPlay();
	//솔직히 불필요한 델리게이트 같다. 그냥 EnemySpawnPool내부에서 타이머를 돌려도 충분하지 않을까?
	ASAGameMode* GameMode = Cast<ASAGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnInitialSpawn.AddUObject(this, &ASAEnemySpawnPool::Spawn);
	GameMode->OnIncreasePoolSize.AddUObject(this, &ASAEnemySpawnPool::IncreasePoolSize);

	//Fill SpawningPool
	for (int32 i = 0; i < MaxPoolPopulation; ++i)
	{
		// SpawnActor의 템플릿을 후에 DataAsset 테이블로 만들어, 생성되는 액터의 다양성을 줄 수 있겠다.
		// Spawn Parameter 사용X <-따로 필요성을 못느껴서
		// [1] 생성자에는 SpawnActor를 못쓰고, [2] 생성자에서 Pawn을 생성하는법을 모르겠고, [3]그렇다고 캐릭터를 CreateDefaultSubobject로 쓸순없으니() 어쩔수없이 BeginPlay에서 생성했다.(초반에만 부담 조금 주지머)
		AEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
			SpawnEnemyClass,
			UKismetMathLibrary::RandomPointInBoundingBox(SpawnField->GetComponentLocation(), SpawnField->GetScaledBoxExtent()),
			GetActorRotation()
		);
		SpawningPool.Emplace(SpawnedCharacter);
	}
}

void ASAEnemySpawnPool::Spawn() //Delegate로 인해 실행
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

	//SpawnDealy시간뒤에 다시 재귀적으로 함수호출
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ASAEnemySpawnPool::Spawn,
		SpawnDelay,
		false //repeat Timer, 이미 함수내에서 재귀적으로 호출하기에 여기는 False해줘야할듯?
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

