#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountPlaytimeSignature, const int, Playtime);

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
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* MainCamera;
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerState CurrentState;

	/** Enhanced Input */
	/** Player Combat InputMappingContext*/
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_PlayerCombat;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Movement;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_BuildMenuTrigger;

	/** Player Build InputMappingContext*/
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* IMC_PlayerBuildMenu;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_BuildComplete;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_BuildCancel;


	void Move(const FInputActionValue& Value);

	void LookCursorDirection();

	void Fire();

	//Make BuildComponent
	UFUNCTION(BlueprintCallable)
	void BuildStart(UClass* InBuildingRef);
	UFUNCTION(BlueprintCallable)
	void BuildComplete();
	UFUNCTION(BlueprintCallable)
	void BuildCancel();
	UPROPERTY()
	AActor* HoldingActor;

	//Make PlayTimeComponent;
	UFUNCTION()
	void CountPlaytime();
	UPROPERTY()
	FTimerHandle PlaytimeTimer;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPriavteAccess = "true"))
	int Playtime;
	UFUNCTION()
	void BroadcastPlaytimeDelegate();

public:
	UPROPERTY(BlueprintAssignable)
	FCountPlaytimeSignature OnCountPlaytime;


};
