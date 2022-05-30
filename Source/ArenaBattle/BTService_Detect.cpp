// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// 서비스 노드는 자신이 속한 컴포짓 노드가 활성화될 경우 주기적으로 TickNode 함수를 호출한다.
// 호출 주기는 서비스 노드 내부에 설정된 Interval 속성 값으로 지정할 수 있다.
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
		return;

	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 300.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, ControllingPawn);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if (ABCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 1.f);
				DrawDebugLine(World, Center, ABCharacter->GetActorLocation(), FColor::Blue, false, 1.f);
				return;
			}
		}
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
