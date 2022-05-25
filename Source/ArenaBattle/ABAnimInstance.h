// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"


/*
AnimInstance.h에 있는 델리게이트 선언
DECLARE_DELEGATE_TwoParams(FOnMontageEnded, UAnimMontage*, bool, bInterrupted);
*/

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // 애님 인스턴스의 Tick 함수

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	void SetDeadAnim() { IsDead = true; }

private:
	// 몽타주 애니메이션을 재생하면 재생 구간에 위치한 노티파이를 호출하게 되고,
	// 노티파이가 호출되면 언리얼 엔진은 이를 보고 자동으로 애님 인스턴스 클래스의
	// 'AnimNotify_노티파이명'이라는 이름의 멤버 함수를 찾아서 호출한다.
	UFUNCTION() // 언리얼 런타임이 찾을 수 있게 UFUNCTION 매크로를 지정한다.
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Pawn)
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Pawn)
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Pawn)
	bool IsDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	UAnimMontage* AttackMontage;

};
