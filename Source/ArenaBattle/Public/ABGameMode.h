// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 게임 규칙을 관리하는 액터
 * 플레이어가 게임에 입장하면 게임 모드에 의해 관련된 액터들이 생성되고 게임플레이를 위한 설정이 갖춰진다.
 * Login()
 * - 플레이어 컨트롤러 생성
 * PostLogin()
 * - 플레이어 폰 생성
 * - 플레이어 컨트롤러가 폰을 빙의 OnPossses(), OnPossessedBy()

 * 게임 시작
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AABGameMode();
	
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void AddScore(class AABPlayerController* ScoredPlayer);
	int32 GetScore() const;

	void EndGame();

private:
	UPROPERTY()
	class AABGameState* ABGameState;

	UPROPERTY()
	int32 ScoreToClear;
};
