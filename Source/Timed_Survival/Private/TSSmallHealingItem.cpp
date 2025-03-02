// Fill out your copyright notice in the Description page of Project Settings.


#include "TSSmallHealingItem.h"

ATSSmallHealingItem::ATSSmallHealingItem()
{
	count = 1; // 아이템 수집 카운트
	HealingAmount = 60.0f; // 기본 회복량
	ItemType = FName(TEXT("SmallHealing")); // 아이템 타입
}

void ATSSmallHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}