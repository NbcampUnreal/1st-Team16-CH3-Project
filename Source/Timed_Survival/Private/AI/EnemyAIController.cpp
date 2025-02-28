// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/BTService_TSSightPlayer.h"

const float AEnemyAIController::PatrolRadius(500.f);
int32 AEnemyAIController::ShowAIDebug(0);
const FName AEnemyAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName AEnemyAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName AEnemyAIController::TargetActorKey(TEXT("TargetActor"));
const FName AEnemyAIController::PlayerDetectedKey(TEXT("PlayerDetected"));


FAutoConsoleVariableRef CVarShowAIDebug(
	TEXT("TSProject.ShowAIDebug"),
	AEnemyAIController::ShowAIDebug,
	TEXT(""),
	ECVF_Cheat
);

AEnemyAIController::AEnemyAIController()
{
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	SetPerceptionComponent(*AIPerception);

	// 시야
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->NearClippingRadius = 0.f;

	// 적, 중립, 아군 감지
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;
	SightConfig->SetMaxAge(3.f);
	SightConfig->SetStartsEnabled(true);

	AIPerception->ConfigureSense(*SightConfig);

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
	}
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndAI();

	Super::EndPlay(EndPlayReason);
}

void AEnemyAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent) == true)
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			bool bRundSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRundSucceeded == true, TEXT("Invalid BehaviorTree"));

			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());

			if (ShowAIDebug == 1)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI()")));
			}

		}
	}
}

void AEnemyAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BehaviorTree);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();

		if (ShowAIDebug == 1)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EndAI()")));
		}
	}
}