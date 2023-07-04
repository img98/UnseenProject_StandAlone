// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UStatComponent;
class AController;

UCLASS()
class PROJECTUNSEEN_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStatComponent* StatComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USAStatData* StatData;

	virtual void IsDead();

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStatComponent* GetStatComponent() { return StatComponent; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
