// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/KismetMathLibrary.h"

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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookCursorDirection();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
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
