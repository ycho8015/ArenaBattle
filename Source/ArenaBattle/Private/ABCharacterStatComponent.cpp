// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

#include "Kismet/GameplayStatics.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true; // InitializeComponent()가 호출되려면 true로 설정해야 한다.
	
	Level = 1;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// 액터의 PostInitializeComponents() 함수가 호출되기 바로 전에 호출된다.
void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) data does not exist."), NewLevel);
	}
}

void UABCharacterStatComponent::SetDamage(float Damage)
{
	ABCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - Damage, 0.f, CurrentStatData->MaxHP));
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	
	// float의 값을 0과 비교할 때는 미세한 오차 범위 내에 있는지를 보고 판단하는 것이 좋다.
	// 언리얼 엔진은 무시 가능한 오차를 측정할 때 사용하도록 KINDA_SMALL_NUMBER 매크로를 제공한다.
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}

float UABCharacterStatComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.f);
	return CurrentStatData->Attack;
}

float UABCharacterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.f);
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / CurrentStatData->MaxHP);
}
