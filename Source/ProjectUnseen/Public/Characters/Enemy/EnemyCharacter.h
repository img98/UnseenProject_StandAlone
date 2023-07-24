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
	TObjectPtr<class UWidgetComponent> HPbarWidget; //HP�ٿ� ����HP�� �������Ʈ���� ��������.

private:

public:
	void SetActivated(bool InBool); //activate ���η����� ���ǵ帮�� ĸ��ȭ��Ű�°� ������ �׳� public�� �ִ°� ������?
	FORCEINLINE bool GetActivated() { return bActivated; }
};
