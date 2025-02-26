#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TSGameInstance.generated.h"

UCLASS()
class TIMED_SURVIVAL_API UTSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UTSGameInstance();

	
			

	// 회복 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 TotalHealingCount;

	// 회복 수집 횟수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToHealingCount(int32 Amount);
};
