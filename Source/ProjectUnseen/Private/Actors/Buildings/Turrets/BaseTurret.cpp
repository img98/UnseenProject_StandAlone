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
			
			//커서의 위치에 따라 지속적으로 이동하기 위해 Tick에 넣었지만, 이전 위치와 일정거리 이상 떨어질 때에만 이벤트가 발생하도록 설정하면 Tick에서 뺄수있겠다.
			SetActorLocation(BuildPosition);
			break;
		}
		case ETurretState::ETS_Searching:
		{
			RotateTurret();
			if (EnemyArray.Num() > 0) //이또한 OnConstruct등을 사용해 내부 프로퍼티가 변하면 State를 변하게 설정해주면 Tick에서 뺄수있다.
			{
				SetTurretState(ETurretState::ETS_InCombat);
			}
			break;
		}
		case ETurretState::ETS_InCombat:
		{
			LookAtEnemy(DeltaTime); //RInterp만을 사용한다면 DeltaTime이 필요없으니 Tick에서 뺄수있다. 그런데 RInterp_Constant를 써서 DeltaTime이 필요한데 이건 못바꿀까? 
									//Spline을쓰면 억지로 interp를 뺄수도있겠다. 아니면 타이머를 사용해서 일시적으로 RInterp_Constant를 사용하거나. 어쨋든 tick보다는 낫다.
									//GetWorld()->GetDeltaSeconds()를 사용하니까 tick에 안넣어도 되더라!!
			if (EnemyArray.Num() < 1)
			{
				SetTurretState(ETurretState::ETS_Searching);
			}
			if (bCanFire)
			{
				Fire(); //raycast 해서 있으면 발사되게해도 괜찮을듯? 아니면 내적을 사용해 일정각도 이하일때만 발사
				FireDelay();
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

	StatComponent->SetStats(StatData);
}

void ABaseTurret::RotateTurret()
{
	//빙빙돌기 or 고개숙이기 . 비주얼적 요소이기에 최후부에 구현하자
}

void ABaseTurret::LookAtEnemy(float DeltaTIme)
{
	if (!EnemyArray.IsValidIndex(0)) return; //이걸 안해주니까 시작하자마자 크래쉬나더라

	AEnemyCharacter* Target = EnemyArray[0].Get(); //약참조를 사용하기에 Get()을 사용해서 유효성을 확인해줘야한다.
	if (Target == nullptr) return;

	const FRotator LookTargetRotator = UKismetMathLibrary::FindLookAtRotation(RotateGunAnchor->GetComponentLocation(), Target->GetActorLocation());
	const FRotator InterpTargetRotator = UKismetMathLibrary::RInterpTo_Constant(RotateGunAnchor->GetComponentRotation(), LookTargetRotator, DeltaTIme, RotateInterpSpeed);
	TurretBodyMesh->SetWorldRotation(FRotator(0.f, InterpTargetRotator.Yaw, 0.f));
	TurretGunMesh->SetRelativeRotation(FRotator(InterpTargetRotator.Pitch, 0.f, 0.f));

	// TODO : Rotate가 현위치에서 목적지까지 부드럽게 움직이는 방법을 고안해보자.

}

void ABaseTurret::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Fire함수의 재정의가 이루어지지 않았습니다. 자식Class에서 정의해주세요"), *GetName());

	bCanFire = false;
}

void ABaseTurret::FireDelay()
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
	FireField->SetCollisionProfileName(TEXT("AggroSpherePreset")); //건설 완료시 공격범위 profile다시 설정해주기
}
