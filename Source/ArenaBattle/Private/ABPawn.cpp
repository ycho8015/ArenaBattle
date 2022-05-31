// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	
	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f),
		FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));

	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애니메이션 블루프린트를 이용해서 캐릭터 애니메이션을 적용한다.
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 스켈레탈 메시 컴포넌트는 자신이 관리하는 캐릭터의 애니메이션을 애님 인스턴스에 위임하는 구조로 설계돼 있다.
	// 스켈레탈 메시가 애니메이션 블루프린트를 실행시키려면 블루프린트 애셋의 클래스 정보를 애님 인스턴스 속성에 지정해줘야 한다.
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Book/Blueprints/BP_WarriorAnim.BP_WarriorAnim_C'"));

	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();

// 게임의 규모가 커지면 이런 방법으로 애니메이션을 재생하는 것은 관리적인 한계에 부딪힌다.
// 언리얼 엔진은 체계적으로 애니메이션 시스템을 설계하도록 별도의 애니메이션 시스템을 제공하는데,
// 이를 애니메이션 블루프린트라고 한다.
#pragma region 런타임에 애니메이션 실행
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("AnimSequence'/Game/Animations/WarriorRun.WarriorRun'"));
	//if (AnimAsset)
	//{
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
#pragma endregion 런타임에 애니메이션 실행
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::UpDown(float AxisValue)
{
	if (AxisValue == 0.f)
		return;

	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void AABPawn::LeftRight(float AxisValue)
{
	if (AxisValue == 0.f)
		return;

	AddMovementInput(GetActorRightVector(), AxisValue);
}

