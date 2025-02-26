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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealingCount;
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


	// 시간(체력) 증가함수 - 아이템 연동
	void IncreaseTime(float Value);

	
	UFUNCTION(BlueprintPure, Category = "Count")
	int32 GetHealingCount() const;

	// 회복 수집 횟수 증가 함수 - 아이템 연동
	UFUNCTION(BlueprintCallable, Category = "Count")
	void IncreaseHealingCount(int32 Amount);
};
