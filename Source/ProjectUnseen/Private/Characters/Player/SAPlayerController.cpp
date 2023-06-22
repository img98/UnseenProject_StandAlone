// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SAPlayerController.h"

void ASAPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	/** 시작과 동시에 마우스가 viewport에 종속되는 것을 생각했으나, GameAndUI로 하니 의미가 없더라.
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
	커서가 위치한 위젯별로 커서 아이콘을 바꿀예정임. 지금은 임시로 보이게만 했다. 나중에 꼭 위치를 수정해주자.
	*/
}
