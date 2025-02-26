#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSPatrolPath.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ATSPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ATSPatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol")
	TArray<AActor*> Waypoints;

	AActor* GetWayPoint(int32 index) const;
	int32 Num() const;
};
