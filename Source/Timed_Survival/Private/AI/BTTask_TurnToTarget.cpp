// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TSCharacter.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	checkf(IsValid(Enemy) == true, TEXT("Invalid Enemy"));

	if (ATSCharacter* Player = Cast<ATSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
	{
		FVector LookVector = Player->GetActorLocation() - Enemy->GetActorLocation();
		LookVector.Z = 0.f;
		FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();
		Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 2.f));

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
