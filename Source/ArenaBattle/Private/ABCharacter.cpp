// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"
#include "ABCharacterWidget.h"
#include "ABAIController.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AABCharacter::AABCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//auto DefaultSetting = GetDefault<UABCharacterSetting>();
	//if (DefaultSetting->CharacterAssets.Num() > 0)
	//{
	//	for (auto CharacterAsset : DefaultSetting->CharacterAssets)
	//	{
	//		ABLOG(Warning, TEXT("Character Asset : %s "), *CharacterAsset.ToString());
	//	}
	//}

	// CDO
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	// Scene Components
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	// Camera
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Book/Blueprints/BP_WarriorAnim.BP_WarriorAnim_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode();

	// Attack
	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));
	AttackRange = 200.f;
	AttackRadius = 50.f;

	// HP UI
	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/Book/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	// AI
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	// NPC의 스켈레탈 메시 애셋을 비동기로 로딩한다.
	if (!IsPlayerControlled())
	{
		auto DefaultSetting = GetDefault<UABCharacterSetting>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

		auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
		if (nullptr != ABGameInstance)
		{
			AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
		}

	}

	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

void AABCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 600.f;
		SpringArm->bUsePawnControlRotation = true; // rotate the arm based on the controller
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		// 언리얼 엔진의 캐릭터 모델은 기본으로 컨트롤 회전의 Yaw 회전(Z축 회전) 값과 폰의 Yaw 회전만 서로 연동되어 있다.
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.f;
		//SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		ArmLengthTo = 800.f;
		ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// 컨트롤 회전이 가리키는 방향으로 캐릭터가 부드럽게 회전한다.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case EControlMode::NPC:
		bUseControllerRotationPitch = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.f)
		{
			// 하나의 벡터 값과 이에 직교하는 나머지 두 축을 구해 회전 행렬을 생성하고, 이와 일치하는 FRotator 값을 구한다.
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	/*
	Delegate
	특정 객체가 해야 할 로직을 다른 객체가 대신 처리할 수 있도록 만드는 보편적인 설계의 개념(publish, broadcast)
	A(캐릭터) 객체가 B(애님 인스턴스) 객체에게 작업 명령을 내릴 떄 B 객체에 A 자신을 등록하고 B의 작업이 끝나면 이때 A에게 알려준다
	보스 몬스터와 미니언들이 있을 때, 보스 몬스터가 죽을 때 미니언들도 같이 소멸

	Dynamic Delegate : 블루프린트 객체와도 연동하는 델리게이트
	Multicast Delegate : 여러 함수에 바인딩시켜 동시에 실행시킬수 있는 델리게이트

	언리얼에서 델리게이트는 C++ 객체에만 사용할 수 있는 델리게이트와 C++, 블루프린트 객체가 모두 사용할 수 있는 델리게이트로 나뉜다.
	블루프린트 오브젝트는 멤버 함수에 대한 정보를 저장하고 로딩하는 직렬화(Serialization) 메커니즘이 들어있기 때문에 일반 C++ 언어가 관리하는
	방법으로 멤버 함수를 관리 할 수 없다. 그래서 블루프린트와 관련된 C++ 함수는 모두 UFUNCTION 매크로를 사용해야 한다.
	*/
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void
		{
			// ABLOG(Warning, TEXT("OnNextAttackCheck"));
			CanNextCombo = false;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				ABAnim->JumpToAttackMontageSection(CurrentCombo);
			}
		});

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void
		{
			ABLOG(Warning, TEXT("OnHPIsZero"));
			ABAnim->SetDeadAnim();
			SetActorEnableCollision(false);
		});
}

float AABCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage: %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::GTA);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

bool AABCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AABCharacter::UpDown(float AxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = AxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float AxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = AxisValue;
		break;
	}
}

void AABCharacter::LookUp(float AxisValue)
{
	if (AxisValue == 0.f)
		return;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(AxisValue);
		break;
	}
}

void AABCharacter::Turn(float AxisValue)
{
	if (AxisValue == 0.f)
		return;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(AxisValue);
		break;
	}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

// 델리게이트에서 호출할 함수
void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();

	OnAttackEnd.Broadcast();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,												// 물리적 충돌이 탐지된 경우 관련된 정보를 담을 구조체
		GetActorLocation(),										// 탐색을 시작할 위치
		GetActorLocation() + GetActorForwardVector() * 200.f,	// 탐색을 끝낼 위치
		FQuat::Identity,										// 탐색에 사용할 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel2,				// 물리 충돌 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(50.f),						// 탐색에 사용할 기본 도형 정보. 구체, 캡슐, 박스를 사용한다.
		Params);												// 탐색 반응을 설정하기 위한 구조체

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 0.5f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif

	if (bResult && HitResult.GetActor())
	{
		ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(
			CharacterStat->GetAttack(),	// 대미지 세기
			DamageEvent,				// 대미지 종류
			GetController(),			// 공격 명령을 내린 가해자; 가해자는 폰이 아니라 폰에게 명령을 내린 플레이어 컨트롤러이다.
			this);						// 대미지 전달을 위해 사용된 도구
	}
}

void AABCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
}
