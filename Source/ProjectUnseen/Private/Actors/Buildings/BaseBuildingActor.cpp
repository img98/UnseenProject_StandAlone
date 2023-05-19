#include "Actors/Buildings/BaseBuildingActor.h"
#include "Components/StatComponent.h"
#include "Components/BoxComponent.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//각 collision과 메쉬들이 camera와 trace collision ignore되게 설정할것!

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	BuildCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildCollision"));
	BuildCollision->SetupAttachment(RootComponent);
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat Component"));
	
}

void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();

	BuildCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseBuildingActor::BuildCollisionBeginOverlap); //constructor에 넣으면 작동안하더라
	BuildCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseBuildingActor::BuildCollisionEndOverlap);

	/** Collision이 존재하는 모든 컴포넌트 추출 후 CollisionProfile 변경 */
	TArray<UActorComponent*> CollisionArray = GetComponentsByClass(UPrimitiveComponent::StaticClass());
	for (UActorComponent* EachComponent : CollisionArray)
	{
		UPrimitiveComponent* EachPrimitiveComponent = Cast<UPrimitiveComponent>(EachComponent);
		EachPrimitiveComponent->SetCollisionProfileName(TEXT("BuildingPreset"));
	}
	BuildCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic")); //나중에 채널파줘야될듯. 일단 임시방편

	/** build시작 시, 초록색 material로 변경*/
	TArray<UActorComponent*> StaticMeshArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* EachComponent : StaticMeshArray)
	{
		UStaticMeshComponent* EachStaticMeshComponent = Cast<UStaticMeshComponent>(EachComponent);
		ChangeMeshMaterialToGreen(EachStaticMeshComponent);
	}
	// ! BuildCollison Overlap바인드 자식클래스에서 해줄것!

}

void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBuildingActor::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:: 모든Mesh들을 ChangeMeshMaterialToRed(InMesh) 사용해주기
	TArray<UActorComponent*> ActorComponentArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* EachComponent : ActorComponentArray)
	{
		UStaticMeshComponent* EachStaticMeshComponent = Cast<UStaticMeshComponent>(EachComponent);
		ChangeMeshMaterialToRed(EachStaticMeshComponent);
	}
}

void ABaseBuildingActor::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TODO:: 모든Mesh들을 ChangeMeshMaterialToGreen(InMesh) 사용해주기
	TArray<UActorComponent*> ActorComponentArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* EachComponent : ActorComponentArray)
	{
		UStaticMeshComponent* EachStaticMeshComponent = Cast<UStaticMeshComponent>(EachComponent);
		ChangeMeshMaterialToGreen(EachStaticMeshComponent);
	}
}

void ABaseBuildingActor::BuildCompleted()
{
	BuildCollision->SetGenerateOverlapEvents(false);

}

void ABaseBuildingActor::ChangeMeshMaterialToGreen(UStaticMeshComponent* InMesh)
{
	checkf(GreenMaterial, TEXT("[%s] GreenMaterial is not Set."), *GetName());

	UStaticMeshComponent* TargetMesh = InMesh;
	TArray<UMaterialInterface*>MaterialArray = TargetMesh->GetMaterials();

	for (int cnt = 0; cnt < MaterialArray.Num(); ++cnt)
	{
		TargetMesh->SetMaterial(cnt,GreenMaterial);
	}
}

void ABaseBuildingActor::ChangeMeshMaterialToRed(UStaticMeshComponent* InMesh)
{
	checkf(RedMaterial, TEXT("[%s] RedMaterial is not Set."), *GetName());

	UStaticMeshComponent* TargetMesh = InMesh;
	TArray<UMaterialInterface*>MaterialArray = TargetMesh->GetMaterials();

	for (int cnt = 0; cnt < MaterialArray.Num(); ++cnt)
	{
		TargetMesh->SetMaterial(cnt, RedMaterial);
	}
}
