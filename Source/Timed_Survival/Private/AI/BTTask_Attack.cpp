// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	checkf(IsValid(Enemy) == true, TEXT("Invalid Enemy"));

	if (Enemy->bIsNowAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	checkf(IsValid(Enemy) == true, TEXT("Invalid Enemy"));

	Enemy->BeginAttack();

	return EBTNodeResult::InProgress;
}


