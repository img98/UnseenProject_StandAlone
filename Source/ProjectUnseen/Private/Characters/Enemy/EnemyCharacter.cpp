// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	/** 어그로 영역이 필요없을것 같다. 어쩌피 캐릭터에게 달려올것이니까
	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->SetCollisionProfileName(TEXT("AggroSpherePreset"));
	*/

	//처음에 Deactivate를 쓰자니 BrainComponent참조가 안되서 크래시나더라
	bActivated = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Activate()
{
	if (bActivated == false)
	{
		bActivated = true;
		AAIController* AIController = Cast<AAIController>(GetController());
		AIController->GetBrainComponent()->RestartLogic();
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
	}
}

void AEnemyCharacter::Deactivate()
{
	bActivated = false;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController->GetBrainComponent()->IsRunning())
	{
		AIController->GetBrainComponent()->StopLogic(FString("Deactivated")); //StopLogic의 인자로 FString Reason을 넣어줘야 하는데, 뭘넣어줘야하지? 그냥 아무거나 써도되나?
	}
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AEnemyCharacter::Dead()
{
	//죽으면 필요한 기능들

}

void AEnemyCharacter::SetActivated(bool InBool)
{
	if (InBool)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}
