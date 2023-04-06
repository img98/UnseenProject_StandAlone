// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/GaussTurret.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AGaussTurret::AGaussTurret()
{
	FireSpeed = 2.5f;
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(TurretGunMesh);
}

void AGaussTurret::Fire()
{
	/** TODO: 무기이펙트 생성 
	UNiagaraFunctionLibrary::SpawnSystemAttached()
	*/
	//지울코드
	bCanFire = true;
}

void AGaussTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGaussTurret::BeginPlay()
{
	Super::BeginPlay();
}
