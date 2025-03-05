#include "AI/BTService_TSSightPlayer.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

UBTService_TSSightPlayer::UBTService_TSSightPlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NodeName = TEXT("SightPlayer");
	Interval = 1.f;
}

void UBTService_TSSightPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController) == true)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
		if (IsValid(Enemy) == true)
		{
            ACharacter* Player = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			FAIStimulus AIStimulus;
            AIStimulus = CanSenseActor(Player, ETS_AISenseT::E_Sight, OwnerComp);
			if (AIStimulus.WasSuccessfullySensed())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetActorKey, Player);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetActorKey, nullptr);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 120.f;
			}
		}
	}
}

FAIStimulus UBTService_TSSightPlayer::CanSenseActor(AActor* Actor, ETS_AISenseT Sense, UBehaviorTreeComponent& OwnerComp)
{
    FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
    FAIStimulus ResultStimulus;

	TSubclassOf<UAISense> QuerySenseClass;

	switch (Sense)
	{
	case ETS_AISenseT::E_None:
		break;
	case ETS_AISenseT::E_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	default:
		break;
	}
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (AIController && AIController->GetAIPerceptionComponent())
    {
        AIController->GetAIPerceptionComponent()->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);
		
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
    }

    return ResultStimulus;
}
