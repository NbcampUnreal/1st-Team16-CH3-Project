// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBigHealingItem.h"

ATSBigHealingItem::ATSBigHealingItem()
{
	count = 1; // 아이템 수집 카운트
	HealingAmount = 300.0f; // 기본 회복량
	ItemType = FName(TEXT("BigHealing")); // 아이템 타입
}

void ATSBigHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
