#include "Actors/Buildings/BaseBuildingActor.h"
#include "Components/StatComponent.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//�� collision�� �޽����� camera�� trace collision ignore�ǰ� �����Ұ�!

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

