// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"

// 소스 코드를 컴파일하기 이전에 UHT를 통해 클래스 선언을 분석하고,
// 언리얼 실행 환경에 필요한 부가 정보를 별도의 파일에 생성한다.
#include "Fountain.generated.h" 

// 언리얼 오브젝트임을 선언
// 언리얼 실행 환경에 의해 관리되는 C++ 객체
UCLASS()
class ARENABATTLE_API/*DLL 내 클래스 정보를 외부에 공개; 없으면 다른 모듈에서 이 객체에 접근할 수 없다*/ AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// UPROPERTY 매크로를 사용하면 해당 언리얼 객체에 대해서 자동으로 메모리 관리를 한다.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* Splash;

	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* Movement;

	// UPROPERTY 매크로로 선언된 값 유형은 초기화 값을 보장받는다.
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

private:
	UPROPERTY(EditAnywhere, Category=Stat, meta=(AllowPrivateAccess=true))
	float RotateSpeed;
};


/*
씬 컴포넌트 : 트랜스폼 정보가 필수적인 컴포넌트
액터 컴포넌트 : 트랜스폼 정보가 없는 컴포넌트

씬 컴포넌트는 액터 컴포넌트를 상속받아 트랜스폼 정보를 추가한 클래스이다.
*/
