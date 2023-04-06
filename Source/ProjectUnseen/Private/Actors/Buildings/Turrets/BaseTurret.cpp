// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "Components/SphereComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ABaseTurret::ABaseTurret()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	TurretRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRootMesh"));
	TurretRootMesh->SetupAttachment(Root);
	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBodyMesh"));
	TurretBodyMesh->SetupAttachment(TurretRootMesh);
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGunMesh"));
	TurretGunMesh->SetupAttachment(TurretBodyMesh);
	FireField = CreateDefaultSubobject<USphereComponent>(TEXT("FireField"));
	FireField->SetupAttachment(GetRootComponent());
	
	TurretRootMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TurretBodyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TurretGunMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TurretState = ETurretState::ETS_Searching;
}

void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (TurretState)
	{
	case ETurretState::ETS_NonCombat:
		break;

	case ETurretState::ETS_Searching:
		RotateTurret();
		break;

	case ETurretState::ETS_InCombat:
		if (EnemyArray.Num() < 1)
		{
			SetTurretState(ETurretState::ETS_Searching);
		}
		Fire();
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("[%s] TurretState is Default!"),*GetName());
		break;
	}
}

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	FireField->OnComponentBeginOverlap.AddDynamic(this, &ABaseTurret::FireFieldBeginOverlap);
	FireField->OnComponentEndOverlap.AddDynamic(this, &ABaseTurret::FireFieldEndOverlap);

}

void ABaseTurret::RotateTurret()
{
	if (!EnemyArray.IsValidIndex(0)) return; //�̰� �����ִϱ� �������ڸ��� ũ����������
	AEnemyCharacter* Target = EnemyArray[0];
	if (Target == nullptr) return;

	FRotator LookTargetRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	TurretBodyMesh->SetWorldRotation(FRotator(0.f, LookTargetRotator.Yaw, 0.f));
	TurretGunMesh->SetRelativeRotation(FRotator(LookTargetRotator.Pitch, 0.f, 0.f));
	//���� GunMesh�� ������ ����� ���� ������ �ִ�. ������ �Ŀ� �ٽ� �����غ���.

	//TODO::Fire ��ġ�� �ű�� �ϴ��� ������Ʈ�� ���� �ڵ�
	Fire();
}

void ABaseTurret::Fire()
{

}

void ABaseTurret::FireFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Emplace(EnemyCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Added!"));
}

void ABaseTurret::FireFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Remove(EnemyCharacter);
	UE_LOG(LogTemp, Warning, TEXT("Removed!"));
}
