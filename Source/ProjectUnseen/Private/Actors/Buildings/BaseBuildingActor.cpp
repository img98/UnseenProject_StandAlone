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
		EachPrimitiveComponent->SetCollisionProfileName(TEXT("BuildingBodyPreset"));
	}
	BuildCollision->SetCollisionProfileName(TEXT("PreBuildPreset")); //나중에 채널파줘야될듯. 일단 임시방편

	/** build시작 시, 초록색 material로 변경*/
	SetAllOverlayMaterials(GreenMaterial);
}

void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBuildingActor::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentBuildState != EBuildState::EBS_BuildCompleted)
	{
		CurrentBuildState = EBuildState::EBS_OnBuildRed;
		SetAllOverlayMaterials(RedMaterial);
	}
}

void ABaseBuildingActor::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrentBuildState != EBuildState::EBS_BuildCompleted)
	{
		CurrentBuildState = EBuildState::EBS_OnBuildGreen;
		SetAllOverlayMaterials(GreenMaterial);
	}
}

void ABaseBuildingActor::BuildCompleted()
{
	//BuildCollision->SetGenerateOverlapEvents(false);
	if (CurrentBuildState != EBuildState::EBS_BuildCompleted)
	{
		CurrentBuildState = EBuildState::EBS_BuildCompleted;
		SetAllOverlayMaterials(nullptr);

		BuildCollision->SetCollisionProfileName(TEXT("GameObjectPreset"));
	}
}

void ABaseBuildingActor::SetAllOverlayMaterials(UMaterialInterface* InMaterial)
{
	TArray<UActorComponent*> ActorComponentArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* EachComponent : ActorComponentArray)
	{
		UStaticMeshComponent* EachStaticMeshComponent = Cast<UStaticMeshComponent>(EachComponent);
		EachStaticMeshComponent->SetOverlayMaterial(InMaterial); //overlay material 비우기
	}
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
