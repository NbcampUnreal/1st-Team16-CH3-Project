// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_TSDetectPlayer.h"
#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
UBTService_TSDetectPlayer::UBTService_TSDetectPlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NodeName = TEXT("DetectPlayer");
	Interval = 1.f;
}

void UBTService_TSDetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController) == true)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
		if (IsValid(Enemy) == true)
		{
			UWorld* World = Enemy->GetWorld();
			if (IsValid(World) == true)
			{
				FVector CenterPosition = Enemy->GetActorLocation();
				float DetectRadius = 300.f;
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, Enemy);
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel12,
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ACharacter* PlayerController = Cast<ACharacter>(OverlapResult.GetActor());
						if (IsValid(PlayerController) == true && PlayerController->GetController()->IsPlayerController() == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetActorKey, PlayerController);

							if (AEnemyAIController::ShowAIDebug == 1)
							{
								UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected")));
								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
								DrawDebugPoint(World, PlayerController->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								DrawDebugLine(World, Enemy->GetActorLocation(), PlayerController->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
							}

							break;
						}
						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetActorKey, nullptr);
							
							if (AEnemyAIController::ShowAIDebug == 1)
							{
								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
							}
						}
					}
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetActorKey, nullptr);
				}

				if (AEnemyAIController::ShowAIDebug == 1)
				{
					DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
				}
			}
		}
	}
}