// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/StatComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	/** ��׷� ������ �ʿ������ ����. ��¼�� ĳ���Ϳ��� �޷��ð��̴ϱ�
	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(GetRootComponent());
	AggroSphere->SetCollisionProfileName(TEXT("AggroSpherePreset"));
	*/

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
		AAIController* AIController = Cast<AAIController>(GetController());
		AIController->GetBrainComponent()->RestartLogic();
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
	}

	StatComponent->SetCurrentHP(StatComponent->GetMaxHP());
}

void AEnemyCharacter::Deactivate()
{
	bActivated = false;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController->GetBrainComponent()->IsRunning())
	{
		AIController->GetBrainComponent()->StopLogic(FString("Deactivated")); //StopLogic�� ���ڷ� FString Reason�� �־���� �ϴµ�, ���־��������? �׳� �ƹ��ų� �ᵵ�ǳ�?
	}
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AEnemyCharacter::IsDead() //����ִϸ��̼� ��� ��, Notify�� ��Ÿ�� ���� ���� IsDead�� ȣ���ϴ°� �����Ͱ���. �ȱ׷��� �ִϸ��̼� �����⵵ ���� Deactivate�ȴ�.
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
