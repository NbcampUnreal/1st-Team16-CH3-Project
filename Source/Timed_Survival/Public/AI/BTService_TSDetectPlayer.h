// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TSDetectPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API UBTService_TSDetectPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_TSDetectPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
