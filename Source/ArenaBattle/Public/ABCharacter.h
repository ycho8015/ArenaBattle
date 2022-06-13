// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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
	AABCharacter();

	// 캐릭터 스테이트 설정
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

protected:
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC,
	};
	void SetControlMode(EControlMode ControlMode = EControlMode::GTA);

	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);

	void Attack();
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

	int32 GetExp() const;

private:
	void UpDown(float AxisValue);
	void LeftRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void ViewChange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	void OnAssetLoadCompleted();

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class AABWeapon* CurrentWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, Category=Stat)
	class UABCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category=UI)
	class UWidgetComponent* HPBarWidget;

public:
	FOnAttackEndDelegate OnAttackEnd;

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

private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category=State, Meta=(AllowPrivateAccess=true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category=State, Meta=(AllowPrivateAccess=true))
	bool bIsPlayer;

	UPROPERTY()
	class AABAIController* ABAIController;

	UPROPERTY()
	class AABPlayerController* ABPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State, Meta=(AllowPrivateAccess=true))
	float DeadTimer;

	int32 AssetIndex = 0;
	FTimerHandle DeadTimerHandle = {};

private:
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
};
