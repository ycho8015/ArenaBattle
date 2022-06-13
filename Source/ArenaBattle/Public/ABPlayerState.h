// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerLevelChangedDelegate, int32);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	int32 GetCurrentGameScore() const;
	int32 GetHighestGameScore() const;
	int32 GetCharacterLevel() const;
	int32 GetCharacterIndex() const;
	float GetExpRatio() const;

	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerData();
	void SavePlayerData();

private:
	void SetCharacterLevel(int32 NewCharacterLevel);

public:
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;
	FOnPlayerLevelChangedDelegate OnPlayerLevelChanged;
	FString SaveSlotName;

protected:
	UPROPERTY(Transient)
	int32 CurrentGameScore;

	UPROPERTY(Transient)
	int32 HighestGameScore;

	UPROPERTY(Transient)
	int32 CharacterIndex;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

private:
	struct FABCharacterData* CurrentStatData;
};
