// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UENUM(BlueprintType)
enum class ETS_AISense : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Sight UMETA(DisplayName = "Sight")
};
/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	// virtual void OnPossess(APawn* InPawn) override;
	// virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;	// MoveToActor 함수가 끝나면 불리는 함수
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void BeginAI(APawn* InPawn);

	void EndAI();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIPerception")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AIPerception")
	UAISenseConfig_Sight* SightConfig;

	static const float PatrolRadius;

	static int32 ShowAIDebug;

	static const FName StartPatrolPositionKey;

	static const FName EndPatrolPositionKey;

	static const FName TargetActorKey;

	/*int32 CurrentPatrolIndex = 0;

	void MoveToPatrolPoint();*/

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	FAIStimulus CanSenseActor(AActor* Actor, ETS_AISense Sense);
};
