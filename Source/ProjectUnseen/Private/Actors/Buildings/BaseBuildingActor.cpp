// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/BaseBuildingActor.h"

// Sets default values
ABaseBuildingActor::ABaseBuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�� collision�� �޽����� camera�� trace collision ignore�ǰ� �����Ұ�!
}

// Called when the game starts or when spawned
void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

