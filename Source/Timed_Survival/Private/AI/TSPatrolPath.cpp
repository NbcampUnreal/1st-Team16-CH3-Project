

#include "AI/TSPatrolPath.h"

ATSPatrolPath::ATSPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

AActor* ATSPatrolPath::GetWayPoint(int32 index) const
{
	if (Waypoints.IsValidIndex(index))
	{
		return Waypoints[index];
	}

	return nullptr;
}

int32 ATSPatrolPath::Num() const
{
	return Waypoints.Num();
}

