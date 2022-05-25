// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

// 캐릭터는 폰을 상속받는 액터로 인간형 폰을 좀 더 효과적으로 제작하기 위한 액터이다.
// 캐릭터 모델이 폰 모델과 다른 점은 캐릭터 무브먼트 컴포넌트를 사용한다.
// - 중력
// - 다양한 움직임 설정(걷기, 기어가기, 날아가기, 수영하기 등)
// - 멀티 플레이 네트워크 환경에서 캐릭터들의 움직임을 자동으로 초기화한다.
UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
	};
	void SetControlMode(EControlMode ControlMode);

	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);

private:
	void UpDown(float AxisValue);
	void LeftRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void ViewChange();
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class AABWeapon* CurrentWeapon = nullptr;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true), Category=Attack)
	float AttackRadius;

	UPROPERTY()
	class UABAnimInstance* ABAnim;
};
