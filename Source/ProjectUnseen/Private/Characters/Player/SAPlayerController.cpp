// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SAPlayerController.h"

void ASAPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	/** ���۰� ���ÿ� ���콺�� viewport�� ���ӵǴ� ���� ����������, GameAndUI�� �ϴ� �ǹ̰� ������.
	FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);
	*/
}

void ASAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Error, TEXT("post initialize"));

	bShowMouseCursor = true;
	CurrentMouseCursor = EMouseCursor::Crosshairs;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void ASAPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Error, TEXT("OnPossess"));

	/**
	Ŀ���� ��ġ�� �������� Ŀ�� �������� �ٲܿ�����. ������ �ӽ÷� ���̰Ը� �ߴ�. ���߿� �� ��ġ�� ����������.
	*/
}
