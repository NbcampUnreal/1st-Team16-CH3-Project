#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TSGameState.generated.h"

class ATSBaseItem;

UCLASS()
class TIMED_SURVIVAL_API ATSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATSGameState();

	virtual void BeginPlay() override;

	//about Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthBarMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BaseHealth;
	float ItemHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 FoodCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealingCount;
	int32 GetRandomValue;

	FTimerHandle SubtractHealthTimerHandle;

	// OnGameOver()에서 애니메이션끝나고 Pause되도록하는 TimerHandle
	FTimerHandle PauseForDeadAnimTimerHandle; //<- 이름 헷갈려서 변수 이름 수정했습니다.

	// CurrentHealth가 지속적으로 감소하는 함수 + 0으로 떨어지는지 확인하기위한 함수 추가
	void SubtractHealthOnSecond();
	
	//about game flow
	TArray<FString> Maplist;
	FString CurrentMapName;
	FString ClearLevelName;
	void StartLevel();
	void OnGameOver();
	void EndLevel();
	void EnterMiddleShelter();
	void NextLevel();
	

	// SubtractHealthOnSecont함수에서 CurrentHealth가 0으로 떨어지면 Character.cpp에 Death()함수 호출
	void OnHPZero();
	void BattleSystem();
	
	// about UI Function 
	TMap<FTimerHandle, FName> WidgetTimerMap;
	FName EventItemType;
	FTimerHandle HUDUpdateTimerHandle;
	void UpdateHUD();
	void PickWidgetbyItemType(FName Type);
	void PopUpWidget(FName ItemType, UUserWidget* ItemWidget, float ViewTime);

	// about Health(Timer) Function - with Item Class
	void IncreaseTime(float Value);
	void ReduceTime(float Value, bool bIgnoreMask = false); // 방독면 무시 여부
		
	// about Health Item - with Item Class
	UFUNCTION(BlueprintPure, Category = "Count")
	int32 GetHealingCount() const;
	UFUNCTION(BlueprintCallable, Category = "Count")
	void IncreaseHealingCount(int32 Amount);
	void UpdateHealth();

	// about Bullet Items
	int32 CurrentBulletCount;
	int32 BulletCountInWeapon;
	int32 MaxBulletCount;
	FTimerHandle BulletDataUpdateTimerHandel;
	void UpdateBulletData();

	/*void FindARBullet();
	void FindPistolBullet();*/

	// 방독면 관련 변수
	bool bIsStopTimeReductionEnabled = true; // 시간 감소 활성화 여부
	bool bIsMaskActive = false; // 마스크 활성화 여부
	float SetMaskEffectTime; // 마스트 효과 시간
	float MaskTimeRemaining; // 마스크 남은 시간
	FTimerHandle MaskEffectTimerHandle; // 마스크 효과 타이머 핸들
	
	// 방독면 관련 함수
	void SetStopTimeReductionEnabled(bool bEnable); // 시간 감소 활성화 함수
	void SetMaskEffect(bool bEnable, float Duration); // 마스크 효과 함수
	bool IsMaskActive() const; // 마스크 활성화 여부
	float GetMaskTimeRemaining() const; // 마스크 남은 시간
	void UpdateMaskTimer(); // 마스크 타이머 업데이트 함수
	void GetMaskDuration(float Value); // UI 세팅 위해서 받아오는 함수
		

};
