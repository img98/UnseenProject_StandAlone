// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/GaussTurret.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/StatComponent.h"

AGaussTurret::AGaussTurret()
{
}

void AGaussTurret::Fire()
{
	Super::Fire();
	/** TODO: ��������Ʈ ���� 
	UNiagaraFunctionLibrary::SpawnSystemAttached()
	*/
	//bCanFire = false;
}

void AGaussTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGaussTurret::BeginPlay()
{
	Super::BeginPlay();
	StatComponent->SetStats(10.f, 10.f, 2.5f, 0.f);
}
