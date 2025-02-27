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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthBarMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BaseHealth;
	float PlusHealth;
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 FoodCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealingCount;
	int32 GetRandomValue;
	FTimerHandle SubtractHealthTimerHandle;
	void SubtractHealthOnSecond();

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
