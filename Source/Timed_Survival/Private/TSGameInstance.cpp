#include "TSGameInstance.h"

UTSGameInstance::UTSGameInstance()
{
	TotalHealingCount = 0;
}

// 회복 수집 횟수
void UTSGameInstance::AddToHealingCount(int32 Amount)
{
	TotalHealingCount += Amount;
}