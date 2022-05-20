// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 컨트롤러에게 UI를 배제하고 게임에게만 입력을 전달하도록 명령한다.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
