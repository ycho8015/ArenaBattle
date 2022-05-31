// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

// INI 파일을 사용해서 캐릭터 애셋 목록을 보관한다.
UCLASS(config="ArenaBattle")
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
public:
	UABCharacterSetting();

public:
	UPROPERTY(config) // 언리얼 오브젝트를 초기화 할 떄 속성의 값을 INI 파일에서 읽어 설정한다.
	TArray<FSoftObjectPath> CharacterAssets;
};
