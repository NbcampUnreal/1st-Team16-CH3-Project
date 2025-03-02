// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSHealingItem.h"
#include "TSMiddleHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSMiddleHealingItem : public ATSHealingItem
{
	GENERATED_BODY()
	
public:
	ATSMiddleHealingItem();



	// 아이템 활성화 시
	virtual void ActivateItem(AActor* Activator) override;
};
