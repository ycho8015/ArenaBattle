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
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void RestartLevel() override;

	class UABHUDWidget* GetHUDWidget() const;
	void ChangeInputMode(bool bGameMode = true);
	void ShowResultUI();

	void NPCKill(class AABCharacter* KilledNPC) const;
	void AddGameScore() const;

protected:
	virtual void BeginPlay() override;

	void OnGamePause();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<class UABGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<class UABGameplayResultWidget> ResultWidgetClass;

private:
	UPROPERTY()
	class AABPlayerState* ABPlayerState;

	UPROPERTY()
	class UABHUDWidget* HUDWidget;

	UPROPERTY()
	class UABGameplayWidget* MenuWidget;

	UPROPERTY()
	class UABGameplayResultWidget* ResultWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
