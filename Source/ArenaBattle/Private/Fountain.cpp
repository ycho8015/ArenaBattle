// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트를 생성한다.
	// TEXT 매크로 : 액터에 속한 컴포넌트를 구별하기 위한 해시 값.
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));

	// RootComponent : 액터를 대표하는 하나의 컴포넌트
	// 모든 액터는 루트 컴포넌트를 갖고 있어야한다.
	RootComponent = Body;

	// 루트 컴포넌트의 자식 컴포넌트
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	// 루트 컴포넌트에서의 상대적 위치
	Water->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	Light->SetRelativeLocation(FVector(0.f, 0.f, 195.f));
	Splash->SetRelativeLocation(FVector(0.f, 0.f, 195.f));

	// 애셋을 관리하는 시스템에 키 값으로 쿼리를 던져 애셋의 포인터를 가져온다.
	// {오브젝트 타입}'{폴더명}/{파일명}.{애셋명}'

	// 애셋은 게임 실행 중 변경될 일이 없기 때문에 static으로 선언해서 한번만 초기화한다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));

	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (SM_WATER.Succeeded())
	{
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SPLASH(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01'"));

	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}

	RotateSpeed = 30.f;
	Movement->RotationRate = FRotator(0.f, RotateSpeed, 0.f);
}

// 액터에 속한 모든 컴포넌트의 세팅이 완료되면 호출된다.
void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddActorLocalRotation(FRotator(0.f, RotateSpeed * DeltaTime, 0.f));
	
}

