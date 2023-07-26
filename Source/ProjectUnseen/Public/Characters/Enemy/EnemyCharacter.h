// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROJECTUNSEEN_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	void Activate();
	void Deactivate();
	UPROPERTY()
	bool bActivated;
	virtual void IsDead() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPriavteAccess = "true"))
	TObjectPtr<class UWidgetComponent> HPbarWidget; //HP바와 스탯HP는 블루프린트에서 연결하자.

private:

public:
	void SetActivated(bool InBool);
	FORCEINLINE bool GetActivated() { return bActivated; }
};
