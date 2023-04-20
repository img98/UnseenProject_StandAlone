// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true; //이걸 해야 InitializeComponent 함수가 호출됨.
}


void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStatComponent::SetStats(float InMaxHP, float InAttackDamage, float InAttackSpeed, float InMovementSpeed)
{
	SetMaxHP(InMaxHP);
	SetCurrentHP(InMaxHP);
	SetAttackDamage(InAttackDamage);
	SetAttackSpeed(InAttackSpeed);
	SetMovementSpeed(InMovementSpeed);
}

float UStatComponent::GetHPRatio()
{
	return CurrentHP / MaxHP;
}
