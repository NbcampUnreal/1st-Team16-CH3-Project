// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TSGetEndPatrolPosition.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API UBTTask_TSGetEndPatrolPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TSGetEndPatrolPosition();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
