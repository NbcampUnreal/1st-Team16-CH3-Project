// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

const float AEnemyAIController::PatrolRadius(500.f);
int32 AEnemyAIController::ShowAIDebug(0);
const FName AEnemyAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName AEnemyAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName AEnemyAIController::TargetActorKey(TEXT("TargetActor"));


FAutoConsoleVariableRef CVarShowAIDebug(
	TEXT("TSProject.ShowAIDebug"),
	AEnemyAIController::ShowAIDebug,
	TEXT(""),
	ECVF_Cheat
);

//void AEnemyAIController::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//}
//
//
//
//void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	Super::OnMoveCompleted(RequestID, Result);
//}

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

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::PerceptionUpdated);
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

void AEnemyAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		FAIStimulus AIStimulus;
		AIStimulus = CanSenseActor(UpdatedActor, EAISense::ETS_Sight);
		if (AIStimulus.WasSuccessfullySensed())
		{
			MoveToActor(
				UpdatedActor,
				1.0f,
				true,
				true,
				false,
				nullptr,
				true
			);
		}
	}
}

FAIStimulus AEnemyAIController::CanSenseActor(AActor* Actor, EAISense Sense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	TSubclassOf<UAISense> QuerySenseClass;

	switch (Sense)
	{
	case EAISense::ETS_None:
		break;
	case EAISense::ETS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;
			return ResultStimulus;
		}
	}

	return ResultStimulus;
}
