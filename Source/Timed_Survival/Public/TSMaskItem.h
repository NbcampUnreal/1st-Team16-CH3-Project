// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSBaseItem.h"
#include "TSMaskItem.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSMaskItem : public ATSBaseItem
{
	GENERATED_BODY()
	
public:
	ATSMaskItem();

protected:

	virtual void ActivateItem(AActor* Activator) override;

	// HP 방어 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	float StopTimeDuration;
};
