// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterSelectWidget.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"
#include "ABPlayerState.h"

#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"


void UABCharacterSelectWidget::NextCharacter(bool bForward /*= true*/)
{
	bForward ? CurrentIndex++ : CurrentIndex--;
	
	CurrentIndex = (CurrentIndex == MaxIndex) ? 0 : (CurrentIndex == -1) ? (MaxIndex - 1) : CurrentIndex;

	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto ABGameInstance = GetWorld()->GetGameInstance<UABGameInstance>();
	//auto ABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	ABCHECK(nullptr != ABGameInstance);
	ABCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void UABCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UABCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnConfirmClicked);
}

void UABCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UABCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UABCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UABSaveGame* NewPlayerData = NewObject<UABSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighestScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto ABPlayerState = GetDefault<AABPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, ABPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gameplay"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}
