#include "Actors/Buildings/BaseBuildingActor.h"
#include "Components/StatComponent.h"
#include "Components/BoxComponent.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//각 collision과 메쉬들이 camera와 trace collision ignore되게 설정할것!

	BuildCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildCollision"));

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat Component"));
}

void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
	BuildCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseBuildingActor::BuildCollisionBeginOverlap);
	BuildCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseBuildingActor::BuildCollisionEndOverlap);

}

void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBuildingActor::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:: 오버랩시 material을 Holo Red 로 만들기

}
void ABaseBuildingActor::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TODO:: 오버랩 종료시 Material을 Holo Green으로 만들기
}

void ABaseBuildingActor::BuildCompleted()
{
	//TODO:: 설치완료 input이 들어오면 delegate 삭제해주기.
}
