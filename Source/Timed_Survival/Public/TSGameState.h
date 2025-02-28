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
	float ItemHealth;
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 FoodCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealingCount;
	int32 GetRandomValue;

	FTimerHandle SubtractHealthTimerHandle;

	// OnGameOver()에서 애니메이션끝나고 Pause되도록하는 TimerHandle
	FTimerHandle TimerHandle;

	// CurrentHealth가 0으로 떨어지는지 확인하기위한 함수
	void SubtractHealthOnSecond();

	void StartLevel();
	void OnGameOver();
	void NextLevel();
	void EndLevel();

	// SubtractHealthOnSecont함수에서 CurrentHealth가 0으로 떨어지면 Character.cpp에 Death()함수 호출
	void OnHPZero();
	void BattleSystem();


	// about UI Function 
	FTimerHandle HUDUpdateTimerHandle;
	void UpdateHUD();


	// 시간(체력) 증가함수 - 아이템 연동
	void IncreaseTime(float Value);

	// 시간(체력) 감소함수 - 아이템 연동
	void ReduceTime(float Value);

	bool bIsStopTimeReductionEnabled = true; // 시간 감소 활성화 여부
	// 시간 감소 활성화 함수
	void SetStopTimeReductionEnabled(bool bEnable);


	UFUNCTION(BlueprintPure, Category = "Count")
	int32 GetHealingCount() const;

	// 회복 수집 횟수 증가 함수 - 아이템 연동
	UFUNCTION(BlueprintCallable, Category = "Count")
	void IncreaseHealingCount(int32 Amount);

	// Count Bullet Items
	void FindARBullet();
	void FindPistolBullet();

	void UpdateHealth();
};
