// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTUNSEEN_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:

// Stats
	UPROPERTY(VisibleAnywhere)
	float _MaxHP = 0.f;
	UPROPERTY(VisibleAnywhere)
	float _CurrentHP = 0.f;
	UPROPERTY(VisibleAnywhere)
	float _AttackDamage = 0.f;
	UPROPERTY(VisibleAnywhere)
	float _AttackSpeed = 0.f;
	UPROPERTY(VisibleAnywhere)
	float _MovementSpeed = 0.f;

public:

	UFUNCTION()
	void SetStats(float InMaxHP, float InAttackDamage, float InAttackSpeed, float InMovementSpeed);

	FORCEINLINE float GetCurrentHP() { return _CurrentHP; }
	FORCEINLINE void SetCurrentHP(float InCurrentHP) { _CurrentHP = InCurrentHP; }
	FORCEINLINE float GetMaxHP() { return _MaxHP; }
	FORCEINLINE void SetMaxHP(float InMaxHP) { _MaxHP = InMaxHP; }
	FORCEINLINE float GetAttackDamage() { return _AttackDamage; }
	FORCEINLINE void SetAttackDamage(float InAttackDamage) { _AttackDamage = InAttackDamage; }
	FORCEINLINE float GetAttackSpeed() { return _AttackSpeed; }
	FORCEINLINE void SetAttackSpeed(float InAttackSpeed) { _AttackSpeed = InAttackSpeed; }
	FORCEINLINE float GetMovementSpeed() { return _MovementSpeed; }
	FORCEINLINE void SetMovementSpeed(float InMovementSpeed) { _MovementSpeed = InMovementSpeed; }
};
