// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/StatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCapsulePreset"));

	HPbarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPbarWidget"));
	HPbarWidget->SetupAttachment(RootComponent);
	HPbarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPbarWidget->SetDrawSize(FVector2d(50.f, 5.f)); //size hard coding
	//HPbarWidget->SetWidgetClass는 BP에서 해줄것

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBrainComponent()->RestartLogic();
			SetActorHiddenInGame(false);
			SetActorEnableCollision(true);
		}
	}

	StatComponent->SetCurrentHP(StatComponent->GetMaxHP());
}

void AEnemyCharacter::Deactivate()
{
	bActivated = false;
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (AIController->GetBrainComponent()->IsRunning())
		{
			AIController->GetBrainComponent()->StopLogic(FString("Deactivated"));
		}
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void AEnemyCharacter::IsDead() //사망애니메이션 재생 후, Notify든 몽타주 델리게이트 등을 통해 IsDead를 호출하는게 좋을것같다. 안그러면 애니메이션 끝나기도 전에 Deactivate된다.
{
	//죽으면 필요한 기능들
	Deactivate();
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
