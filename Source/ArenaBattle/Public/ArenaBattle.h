// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" // IWYU

/*
PREINIT
- 캐릭터 생성 전의 스테이트
- 기본 캐릭터 애셋이 설정돼 있지만 캐릭터와 UI를 숨겨둔다.
- 대미지를 입지 않는다.

LOADING
- 선택한 캐릭터 애셋을 로딩하는 스테이트.
- 게임이 시작된 시점이므로 현재 조종하는 컨트롤러가 AI인지 플레이어인지 구분할 수 있다.
- 플레이어 컨트롤러인 경우 애셋 로딩이 완료될 떄까지 캐릭터를 조종하지 못하도록 입력을 비활성화한다.

READY
- 캐릭터 애셋이 로딩이 완료된 스테이트.
- 숨겨둔 캐릭터와 UI를 보여준다.
- 공격을 받을 수 있다.
- 플레이어 컨트롤러 활성화
- AI 컨트롤러 BT 활성화

DEAD
- 캐릭터가 HP를 소진해 사망할 떄 발생하는 스테이트
- 죽는 애니메이션을 재생한다
- UI/충돌/대미지 비활성화
- 컨트롤러 비활성화
- 플레이어는 레벨 재시작
- AI는 레벨 퇴장
*/

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }