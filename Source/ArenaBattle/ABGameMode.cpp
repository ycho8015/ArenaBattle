// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABPawn.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// 객체들을 미리 만들어두는 것이 아니라 플레이어가 입장할 때마다 해당 클래스 정보를 기반으로 객체를 생성한다.
	// Auto Possess Player 속성을 사용하면 레벨에 이미 배치되어 있는 폰에 플레이어 컨트롤러가 빙의할 수 있다.
	// Player 0 : 로컬 플레이어를 의미한다.
	DefaultPawnClass = AABPawn::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
