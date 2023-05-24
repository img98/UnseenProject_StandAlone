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
	float MaxHP;
	UPROPERTY(VisibleAnywhere)
	float CurrentHP;
	UPROPERTY(VisibleAnywhere)
	float AttackDamage;
	UPROPERTY(VisibleAnywhere)
	float AttackSpeed;
	UPROPERTY(VisibleAnywhere)
	float MovementSpeed;

public:

	UFUNCTION(BlueprintCallable)
	float GetHPRatio();
	
	UFUNCTION(BlueprintCallable)
	void SetStats(float InMaxHP, float InAttackDamage, float InAttackSpeed, float InMovementSpeed);

	FORCEINLINE float GetCurrentHP() { return CurrentHP; }
	FORCEINLINE void SetCurrentHP(float InCurrentHP) { CurrentHP = InCurrentHP; }
	FORCEINLINE void IncreaseCurrentHP(float InHP) { CurrentHP = (CurrentHP + InHP) < MaxHP ? CurrentHP + InHP : MaxHP; }
	FORCEINLINE void DecreaseCurrentHP(float InHP) { CurrentHP = (CurrentHP - InHP) > 0 ? CurrentHP - InHP : 0; }
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE void SetAttackDamage(float InAttackDamage) { AttackDamage = InAttackDamage; }
	FORCEINLINE float GetAttackSpeed() { return AttackSpeed; }
	FORCEINLINE void SetAttackSpeed(float InAttackSpeed) { AttackSpeed = InAttackSpeed; }
	FORCEINLINE float GetMovementSpeed() { return MovementSpeed; }
	FORCEINLINE void SetMovementSpeed(float InMovementSpeed) { MovementSpeed = InMovementSpeed; }
};
