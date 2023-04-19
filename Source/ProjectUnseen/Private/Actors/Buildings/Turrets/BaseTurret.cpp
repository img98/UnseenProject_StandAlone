// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildings/Turrets/BaseTurret.h"
#include "Components/SphereComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Projectiles/BaseProjectile.h"

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
	RotateGunAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("RotateGunAnchor"));
	RotateGunAnchor->SetupAttachment(TurretGunMesh);
	ProjectileSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(ProjectileSpawner);
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
		{
			break;
		}
	case ETurretState::ETS_Searching:
		{
			RotateTurret();
			break;
		}
	case ETurretState::ETS_InCombat:
		{
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

void ABaseTurret::RotateTurret()
{
	if (!EnemyArray.IsValidIndex(0)) return; //이걸 안해주니까 시작하자마자 크래쉬나더라
	AEnemyCharacter* Target = EnemyArray[0];
	if (Target == nullptr) return;

	FRotator LookTargetRotator = UKismetMathLibrary::FindLookAtRotation(RotateGunAnchor->GetComponentLocation(), Target->GetActorLocation());
	TurretBodyMesh->SetWorldRotation(FRotator(0.f, LookTargetRotator.Yaw, 0.f));
	TurretGunMesh->SetRelativeRotation(FRotator(LookTargetRotator.Pitch, 0.f, 0.f));

	// TODO : Rotate가 현위치에서 목적지까지 부드럽게 움직이는 방법을 고안해보자.

}

void ABaseTurret::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Fire함수의 재정의가 이루어지지 않았습니다. 자식Class에서 정의해주세요"), *GetName());

	bCanFire = false;
	if (ProjectileClass)
	{
		// 씬컴포넌트로해서 따로 트랜스폼 안따줘도 될듯? //FTransform ProjectileSpawnTransform = ProjectileSpawner->GetComponentTransform();
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());
		FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = TODO ; 
		SpawnParams.Instigator = InstigatorPawn;

		GetWorld()->SpawnActor<ABaseProjectile>(
			ProjectileClass,
			ProjectileSpawner->GetComponentLocation(),
			ProjectileSpawner->GetComponentRotation(),
			SpawnParams
		);
	}
}

void ABaseTurret::FireDelay(float DeltaTime)
{
	//TODO :: 각 터렛들의 프로퍼티인 'FireSpeed'초 후에 bCanFire = true 로 바꾸기

	/** 
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 내가 원하는 코드 구현
			DoSomething();

			// 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(fadeOutTimerHandle);
		}), InDelayTime, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
	*/

	GetWorld()->GetTimerManager().SetTimer(

	);
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
