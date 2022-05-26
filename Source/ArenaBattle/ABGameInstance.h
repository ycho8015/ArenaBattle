// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/*
캐릭터 스탯 데이터는 게임의 기반을 이루는 변하지 않는 데이터이므로 보통 게임 앱이 초기화 될 떄 불러들인다.
게임 인스턴스가 캐릭터의 스탯을 관리하도록 설계하면 게임 앱이 초기화될 때 캐릭터 스탯 데이터를 불러들이고,
게임 엡이 종료될 떄까지 캐릭터 스탯 데이터는 보존된다.
*/
USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 테이블의 첫 번째에 위치한 열 데이터는 언리얼 엔진에서 자동으로 키 값으로 사용하기 때문에 선언에서 제외한다.
	FABCharacterData() 
		: Level(1), MaxHP(100.f), Attack(10.f), DropExp(10), NextExp(30) 
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 NextExp;
};

/*
 게임 앱을 관리하는 언리얼 오브젝트
 */
UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();

	virtual void Init() override;
	FABCharacterData* GetABCharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* ABCharacterTable; // 테이블 데이터를 관리하는 언리얼 오브젝트
};
