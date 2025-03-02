// Fill out your copyright notice in the Description page of Project Settings.


#include "TSMiddleHealingItem.h"

ATSMiddleHealingItem::ATSMiddleHealingItem()
{
	count = 1; // 아이템 수집 카운트
	HealingAmount = 180.0f; // 기본 회복량
	ItemType = FName(TEXT("MiddleHealing")); // 아이템 타입
}

void ATSMiddleHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}