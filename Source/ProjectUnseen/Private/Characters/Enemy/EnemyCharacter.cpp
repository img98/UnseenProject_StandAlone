// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
