// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TSGetEndPatrolPosition.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TSGetEndPatrolPosition::UBTTask_TSGetEndPatrolPosition()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTTask_TSGetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	checkf(IsValid(Enemy) == true, TEXT("Invalid Enemy"));

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(Enemy->GetWorld());
	checkf(IsValid(NavigationSystem) == true, TEXT("Invalid NavigationSystem"));

	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::StartPatrolPositionKey);
	FNavLocation EndPatrolPosition;
	if (true == NavigationSystem->GetRandomPointInNavigableRadius(StartPatrolPosition, AIController->PatrolRadius, EndPatrolPosition))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::EndPatrolPositionKey, EndPatrolPosition.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
