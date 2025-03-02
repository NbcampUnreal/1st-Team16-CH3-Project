// Fill out your copyright notice in the Description page of Project Settings.


#include "TSHealingItem.h"
#include "TSGameState.h" // GameState 헤더 포함
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


ATSHealingItem::ATSHealingItem()
{
	count =0; // 아이템 수집 횟수
	HealingAmount = 10.0f; // 기본 회복량
	ItemType = FName(TEXT("DefaultHealing")); // 아이템 타입
}

void ATSHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	// 플레이어가 맞는지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// GameState 가져오기
		ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GameState)
		{
			GameState->IncreaseTime(HealingAmount); // 시간(체력) 증가
			GameState->IncreaseHealingCount(count); // 음식 수집 횟수 증가
		}

		// 아이템 제거
		DestroyItem();
	}
}