// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TSSightPlayer.generated.h"

//UENUM(BlueprintType)
//enum class ETS_AISense : uint8
//{
//	E_None UMETA(DisplayName = "None"),
//	E_Sight UMETA(DisplayName = "Sight")
//};
/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API UBTService_TSSightPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_TSSightPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


	//UFUNCTION()
	//FAIStimulus CanSenseActor(AActor* Actor, EAISense Sense);
public:
	//UFUNCTION()
	//void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
