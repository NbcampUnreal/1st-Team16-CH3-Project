// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_TSSightPlayer.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

UBTService_TSSightPlayer::UBTService_TSSightPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = TEXT("SightPlayer");
	Interval = 1.f;
}

//void UBTService_TSSightPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
//
//	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
//	if (IsValid(AIController) == true)
//	{
//		AIController->AIPerception->OnPerceptionUpdated.AddDynamic(this, &UBTService_TSSightPlayer::PerceptionUpdated);
//		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
//		if (IsValid(Enemy) == true)
//		{
//			UWorld* World = Enemy->GetWorld();
//			if (IsValid(World) == true)
//			{
//
//			}
//		}
//	}
//}

//void UBTService_TSSightPlayer::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
//{
//	for (AActor* UpdatedActor : UpdatedActors)
//	{
//		FAIStimulus AIStimulus;
//		AIStimulus = CanSenseActor(UpdatedActor, EAISense::E_Sight);
//		if (AIStimulus.WasSuccessfullySensed())
//		{
//
//		}
//	}
//}
//
//FAIStimulus UBTService_TSSightPlayer::CanSenseActor(AActor* Actor, EAISense Sense)
//{
//    FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
//    FAIStimulus ResultStimulus;
//
//    AEnemyAIController* AIController = Cast<AEnemyAIController>(Actor->GetInstigatorController());
//	if (IsValid(AIController) == true)
//    {
//		AIController->AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);
//
//		TSubclassOf<UAISense> QuerySenseClass;
//
//		switch (Sense)
//		{
//		case EAISense::E_None:
//			break;
//		case EAISense::E_Sight:
//			QuerySenseClass = UAISense_Sight::StaticClass();
//			break;
//		default:
//			break;
//		}
//
//		TSubclassOf<UAISense> LastSensedStimulusClass;
//
//		for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
//		{
//			LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);
//
//			if (QuerySenseClass == LastSensedStimulusClass)
//			{
//				ResultStimulus = AIStimulus;
//				return ResultStimulus;
//			}
//		}
//
//		return ResultStimulus;
//    }
//
//    return FAIStimulus();
//}

