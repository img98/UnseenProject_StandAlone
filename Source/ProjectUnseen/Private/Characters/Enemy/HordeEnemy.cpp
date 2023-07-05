// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/HordeEnemy.h"
#include "AI/SAAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AHordeEnemy::AHordeEnemy()
{
	AIControllerClass = ASAAIController::StaticClass();

	PrimaryActorTick.bCanEverTick = false; //Tick을 제거해 성능향상을 노림
	GetCharacterMovement()->MaxWalkSpeed = 180.f;
}

void AHordeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordeEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GetController()))
	{
		UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
		Blackboard->SetValueAsObject(FName("TargetActor"), UGameplayStatics::GetPlayerCharacter(this, 0));
	}
}
