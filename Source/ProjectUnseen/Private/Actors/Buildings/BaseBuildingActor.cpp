#include "Actors/Buildings/BaseBuildingActor.h"
#include "Components/StatComponent.h"
#include "Components/BoxComponent.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//�� collision�� �޽����� camera�� trace collision ignore�ǰ� �����Ұ�!

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
	//TODO:: �������� material�� Holo Red �� �����

}
void ABaseBuildingActor::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TODO:: ������ ����� Material�� Holo Green���� �����
}

void ABaseBuildingActor::BuildCompleted()
{
	//TODO:: ��ġ�Ϸ� input�� ������ delegate �������ֱ�.
}
