// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Shooting UMETA(DisplayName = "Shooting"),
	EPS_Build UMETA(DisplayName = "Build"),
	EPS_Dead UMETA(DisplayName = "Dead"),

	EPS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTUNSEEN_API APlayerCharacter : public ABaseCharacter
{

	GENERATED_BODY()

public:
	APlayerCharacter();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetPlayerState(EPlayerState InState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* MainCamera;
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerState CurrentState;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* IMC_PlayerCombat;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* IMC_PlayerBuildMenu;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Movement;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_BuildMenuTrigger;


	void Move(const FInputActionValue& Value);

	void LookCursorDirection();

	void Fire();

	UFUNCTION(BlueprintCallable)
	void BuildMenuTrigger(UInputMappingContext* InIMC);

	UFUNCTION(BlueprintCallable)
	void BuildStart(UClass* InBuildingRef);
	UFUNCTION(BlueprintCallable)
	void BuildComplete();
	UPROPERTY()
	AActor* HoldingActor;

private:


};
