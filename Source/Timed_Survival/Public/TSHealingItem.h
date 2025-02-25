// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSBaseItem.h"
#include "TSHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSHealingItem : public ATSBaseItem
{
	GENERATED_BODY()
	
public:
	ATSHealingItem();


protected:
	// 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	float HealingAmount;

	// 아이템 활성화 시 (오버라이드)
	virtual void ActivateItem(AActor* Activator) override;
};
