// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StatComponent.h"

APlayerCharacter::APlayerCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1400.f;
	SpringArm->SetRelativeRotation(FRotator(-75.f, 0.f, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 30.f;
	SpringArm->bInheritYaw = false;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SetPlayerState(EPlayerState::EPS_MAX);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_PlayerCombat, 0); //뒤 숫자는 우선순위를 의미하는듯
		//Subsystem->RemoveMappingContext(IMC_PlayerCombat)
	}

	SetPlayerState(EPlayerState::EPS_Shooting);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookCursorDirection();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(IA_BuildMenuTrigger, ETriggerEvent::Triggered, this, &APlayerCharacter::BuildMenuTrigger);
	EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);


}

void APlayerCharacter::SetPlayerState(EPlayerState InState)
{
	if (CurrentState != EPlayerState::EPS_Dead)
	{
		CurrentState = InState;
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ControlXDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ControlXDirection, MovementVector.Y);
	const FVector ControlYDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ControlYDirection, MovementVector.X);
}



void APlayerCharacter::LookCursorDirection()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

	const FRotator CharacterLookRotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResult.Location);
	const FRotator TargetRotator(0.f, CharacterLookRotator.Yaw, 0.f);
	this->SetActorRotation(TargetRotator);
	//다만, 이경우 고저차가 있는곳위로 커서가 움직일때 Jerking현상이 있더라. Interp등으로 나중에 보완해야될듯
}

void APlayerCharacter::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Fire!"));
}

void APlayerCharacter::BuildMenuTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("Build Action Trigger activated!"));
}
