// Fill out your copyright notice in the Description page of Project Settings.


#include "TSHealingItem.h"
#include "TSGameState.h" // GameState 헤더 포함
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


ATSHealingItem::ATSHealingItem()
{
	HealingAmount = 10.0f; // 기본 회복량 (작은 회복 아이템 값)
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
			GameState->IncreaseTime(HealingAmount);
		}

		// 아이템 제거
		DestroyItem();
	}
}