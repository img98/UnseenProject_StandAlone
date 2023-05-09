// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/GhostTurret.h"

// Sets default values
AGhostTurret::AGhostTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhostTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

