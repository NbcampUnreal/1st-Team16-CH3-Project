#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TSGameState.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ATSGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATSGameState();

	virtual void BeginPlay() override;

	//about Game flow
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 FoodCount;
	int32 GetRandomValue;

	void StartLevel();
	void OnGameOver();
	void NextLevel();
	void EndLevel();
	void OnHPZero();
	void BattleSystem();


	// about UI Function 
	FTimerHandle HUDUpdateTimerHandle;
	void UpdateHUD();
};
