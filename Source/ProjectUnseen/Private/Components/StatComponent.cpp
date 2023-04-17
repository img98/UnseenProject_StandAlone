// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true; //�̰� �ؾ� InitializeComponent �Լ��� ȣ���.
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
	SetAttackSpeed(InAttackDamage);
	SetAttackSpeed(InAttackSpeed);
	SetMovementSpeed(InMovementSpeed);
}