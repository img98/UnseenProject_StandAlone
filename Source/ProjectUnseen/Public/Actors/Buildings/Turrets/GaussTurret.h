// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "GaussTurret.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTUNSEEN_API AGaussTurret : public ABaseTurret
{
	GENERATED_BODY()
	
public:
	AGaussTurret();

	virtual void Fire() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;
};
