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
	//HPbarWidget->SetWidgetClass�� BP���� ���ٰ�

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//ó���� Deactivate�� ���ڴ� BrainComponent������ �ȵǼ� ũ���ó�����
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

void AEnemyCharacter::IsDead() //����ִϸ��̼� ��� ��, Notify�� ��Ÿ�� ��������Ʈ ���� ���� IsDead�� ȣ���ϴ°� �����Ͱ���. �ȱ׷��� �ִϸ��̼� �����⵵ ���� Deactivate�ȴ�.
{
	//������ �ʿ��� ��ɵ�
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
