// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/HordeEnemy.h"

AHordeEnemy::AHordeEnemy()
{
	/** AIControllerClass할당이 프로퍼티로 되있더라. 이걸 Bp에서 할당해줘야할까 아님 header에서 할당해서 넣어야할까?*/
	// AIControllerClass = 
}

void AHordeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordeEnemy::BeginPlay()
{
	Super::BeginPlay();
}
