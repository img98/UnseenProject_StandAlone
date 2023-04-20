#include "Actors/Buildings/BaseBuildingActor.h"
#include "Components/StatComponent.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//각 collision과 메쉬들이 camera와 trace collision ignore되게 설정할것!

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat Component"));
}

void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

