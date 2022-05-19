// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 게임 세계에서 현실 세계의 플레이어를 대변하는 무형의 액터
 * 플레이어 컨트롤러는 게임 세계에서 플레이어와 1:1로 소통하면서 폰을 조종하는 역할을 맡는다.
 * 플레이어 컨트롤러는 현재 조종(빙의)하는 폰을 버리고 다른 폰으로 옮겨가서 조종할 수 있다.
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

};
