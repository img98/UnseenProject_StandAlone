// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "Components/SphereComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Projectiles/BaseProjectile.h"
#include "Components/StatComponent.h"
#include "Components/BoxComponent.h"


ABaseTurret::ABaseTurret()
{
	TurretRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRootMesh"));
	TurretRootMesh->SetupAttachment(Root);
	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBodyMesh"));
	TurretBodyMesh->SetupAttachment(TurretRootMesh);
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGunMesh"));
	TurretGunMesh->SetupAttachment(TurretBodyMesh);
	RotateGunAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RotateGunAnchor"));
	RotateGunAnchor->SetupAttachment(TurretGunMesh);
	FireField = CreateDefaultSubobject<USphereComponent>(TEXT("FireField"));
	FireField->SetupAttachment(GetRootComponent());


	TurretState = ETurretState::ETS_OnBuild;
}

void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretBehaviorStateMachine(DeltaTime);
}

void ABaseTurret::TurretBehaviorStateMachine(float DeltaTime)
{
	switch (TurretState)
	{
		case ETurretState::ETS_OnBuild:
		{
			FHitResult CurorHitResult;
			UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, CurorHitResult); // 후에 Build용 traceChannel만들어서 바꿀것, traceComplex먼지몰라서 true
			
			float InGridSize = 50.f;
			FVector BuildPosition = UKismetMathLibrary::Vector_SnappedToGrid(CurorHitResult.Location, InGridSize);

			SetActorLocation(BuildPosition);
			break;
		}
		case ETurretState::ETS_Searching:
		{
			RotateTurret();
			if (EnemyArray.Num() > 0)
			{
				SetTurretState(ETurretState::ETS_InCombat);
			}
			break;
		}
		case ETurretState::ETS_InCombat:
		{
			LookAtEnemy(DeltaTime);
			if (EnemyArray.Num() < 1)
			{
				SetTurretState(ETurretState::ETS_Searching);
			}
			if (bCanFire)
			{
				Fire();
				FireDelay(DeltaTime);
			}
			break;
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] TurretState is Default!"), *GetName());
			break;
		}
	}
}

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	FireField->OnComponentBeginOverlap.AddDynamic(this, &ABaseTurret::FireFieldBeginOverlap);
	FireField->OnComponentEndOverlap.AddDynamic(this, &ABaseTurret::FireFieldEndOverlap);

}

void ABaseTurret::BuildCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BuildCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ChangeMeshMaterialToRed(TurretRootMesh);
	ChangeMeshMaterialToRed(TurretBodyMesh);
	ChangeMeshMaterialToRed(TurretGunMesh);
}

void ABaseTurret::BuildCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::BuildCollisionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	ChangeMeshMaterialToGreen(TurretRootMesh);
	ChangeMeshMaterialToGreen(TurretBodyMesh);
	ChangeMeshMaterialToGreen(TurretGunMesh);
}

void ABaseTurret::RotateTurret()
{
	// TODO: 의미없이 빙빙돌기 구현하기 or 고개숙이고 대기하기 구현
}

void ABaseTurret::LookAtEnemy(float DeltaTIme)
{
	if (!EnemyArray.IsValidIndex(0)) return; //이걸 안해주니까 시작하자마자 크래쉬나더라

	AEnemyCharacter* Target = EnemyArray[0];
	if (Target == nullptr) return;

	const FRotator LookTargetRotator = UKismetMathLibrary::FindLookAtRotation(RotateGunAnchor->GetComponentLocation(), Target->GetActorLocation());
	const FRotator InterpTargetRotator = UKismetMathLibrary::RInterpTo(RotateGunAnchor->GetComponentRotation(), LookTargetRotator, DeltaTIme, RotateInterpSpeed);
	TurretBodyMesh->SetWorldRotation(FRotator(0.f, InterpTargetRotator.Yaw, 0.f));
	TurretGunMesh->SetRelativeRotation(FRotator(InterpTargetRotator.Pitch, 0.f, 0.f));

	// TODO : Rotate가 현위치에서 목적지까지 부드럽게 움직이는 방법을 고안해보자.

}

void ABaseTurret::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Fire함수의 재정의가 이루어지지 않았습니다. 자식Class에서 정의해주세요"), *GetName());

	bCanFire = false;
}

void ABaseTurret::FireDelay(float DeltaTime)
{
	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		FTimerDelegate::CreateLambda([&]() {
			//TODO : Just Wait for 'Firespeed' seconds & ClearTimer
			bCanFire = true;
			GetWorld()->GetTimerManager().ClearTimer(FireTimer);
			}),
		StatComponent->GetAttackSpeed(),
		false
	);
}

void ABaseTurret::FireFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Emplace(EnemyCharacter);
}

void ABaseTurret::FireFieldEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter == nullptr) return;

	EnemyArray.Remove(EnemyCharacter);
}

void ABaseTurret::BuildCompleted()
{
	Super::BuildCompleted();
	TurretState = ETurretState::ETS_Searching;

	FireField->SetCollisionProfileName(TEXT("OverlapAllDynamic")); //건설 완료시 공격범위 profile다시 설정해주기
}
