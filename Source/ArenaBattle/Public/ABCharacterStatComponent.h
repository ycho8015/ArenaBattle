// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 Level);
	void SetDamage(float Damage);
	void SetHP(float NewHP);
	float GetAttack();
	float GetHPRatio();

public:
	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private:
	struct FABCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Meta=(AllowPrivateAccess=true), Category=Stat)
	int32 Level;

	// 언리얼 오브젝트는 직렬화 기능이 있어서 오브젝트의 UPROPERTY 속성을 저장하고 로딩할 수 있다.
	// CurrentHP 값은 게임을 시작할 때마다 변경되므로 이 값을 보관하는 것은 의미가 없고
	// 오히려 오브젝트를 저장할 떄 필요 없는 디스크 공간만 차지한다.
	// 이러한 속성에는 Transient 키워드를 추가해 해당 속성을 직렬화에서 제외시키는 것이 좋다.
	UPROPERTY(Transient, VisibleInstanceOnly, Meta=(AllowPrivateAccess=true), Category=Stat)
	float CurrentHP;
};
