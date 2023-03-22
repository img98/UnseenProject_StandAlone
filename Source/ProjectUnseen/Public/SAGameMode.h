// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUNSEEN_API ASAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASAGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
