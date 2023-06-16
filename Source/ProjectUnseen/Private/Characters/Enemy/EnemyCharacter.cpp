// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->SetCollisionProfileName(TEXT("AggroSpherePreset"));
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
