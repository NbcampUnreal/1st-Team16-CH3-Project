// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSHealingItem.h"
#include "TSBigHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSBigHealingItem : public ATSHealingItem
{
	GENERATED_BODY()
	

public:
	ATSBigHealingItem();


	// 아이템 활성화 시
	virtual void ActivateItem(AActor* Activator) override;
};
