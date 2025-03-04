// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsInAttackRange.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "TSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

const float UBTDecorator_IsInAttackRange::AttackRange = 200.f;

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false"));

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	checkf(IsValid(Enemy) == true, TEXT("Invalid Enemey"));

	ATSCharacter* TargetPlayer = Cast<ATSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetActorKey));
	if (IsValid(TargetPlayer) == true && TargetPlayer->IsPlayerControlled() == true)
	{
		return Enemy->GetDistanceTo(TargetPlayer) <= AttackRange;
	}

	return false;
}
