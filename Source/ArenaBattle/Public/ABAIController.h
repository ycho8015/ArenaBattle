// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/*
비헤이비어 트리 : 블랙보드 데이터에 기반해 동작하는 트리
블랙보드 : 비헤이비어 트리에서 AI의 판단에 사용하는 데이터
컴포짓 : 노드의 분기가 실행되는 기본 규칙을 정의
- 셀렉터 : 자손 노드 중 하나가 실행에 성공하면 멈춘다.
- 시퀀스 : 자손 노드 중 하나가 실행에 실패하면 멈춘다.

노드
서비스 : 컴포짓에 부착되는 노드, 컴포짓에 속한 태스크들이 실행되는 동안 정해진 빈도에 맞춰 실행된다.
태스크 : 블랙보드의 값 조정과 같은 작업을 하는 노드
*/
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName ItemKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
