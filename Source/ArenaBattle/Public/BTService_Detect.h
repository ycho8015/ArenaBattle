// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 서비스 노드는 독립적으로 동작하지 않고 컴포짓 노드에 부착되는 노드다.
 서비스 노드는 해당 컴포짓에 속한 태스크들이 실행되는 동안 반복적인 작업을 실행하는 데 적합하다.
 */
UCLASS()
class ARENABATTLE_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
