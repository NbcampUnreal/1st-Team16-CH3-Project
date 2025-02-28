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

	// CurrentHealth가 지속적으로 감소하는 함수 + 0으로 떨어지는지 확인하기위한 함수 추가
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

	// about Health(Timer) Function - with Item Class
	void IncreaseTime(float Value);
	void ReduceTime(float Value);
	bool bIsStopTimeReductionEnabled = true; // 시간 감소 활성화 여부
	// 시간 감소 활성화 함수
	void SetStopTimeReductionEnabled(bool bEnable);


	// about Health Item - with Item Class
	UFUNCTION(BlueprintPure, Category = "Count")
	int32 GetHealingCount() const;
	UFUNCTION(BlueprintCallable, Category = "Count")
	void IncreaseHealingCount(int32 Amount);
	void UpdateHealth();

	// about Bullet Items
	void GetBulletData();
	void UpdateBulletData();

	/*void FindARBullet();
	void FindPistolBullet();*/

	
};
