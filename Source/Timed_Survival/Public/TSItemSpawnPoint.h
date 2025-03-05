// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSItemSpawnPoint.generated.h"

// 아이템 타입 Enum(어떤 아이템을 스폰할지 결정)
UENUM(BlueprintType)
enum class ETSItemType : uint8
{
    Mine UMETA(DisplayName = "Mine"),
    Healing UMETA(DisplayName = "Healing"),
    Mask UMETA(DisplayName = "Mask")
};

UCLASS()
class TIMED_SURVIVAL_API ATSItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:
    ATSItemSpawnPoint();

    // 에디터에서 변경할 수 있는 아이템 종류
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    ETSItemType ItemType;

    // 아이템 스폰 확률 (0~100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    float SpawnProbability;

    // 블루프린트에서 설정할 힐링 아이템 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    TSubclassOf<class ATSBaseItem> BigHealingBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    TSubclassOf<class ATSBaseItem> MiddleHealingBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    TSubclassOf<class ATSBaseItem> SmallHealingBP;

    // 힐링 아이템 개별 확률
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    float BigHealingProbability;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    float MiddleHealingProbability;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Settings")
    float SmallHealingProbability;

    void SpawnItem();

    // 스폰된 아이템을 추적할 변수
    UPROPERTY()
    AActor* SpawnedItem;

private:
    // 스폰할 아이템 클래스를 반환하는 함수
    TSubclassOf<class ATSBaseItem> GetItemClass();
    TSubclassOf<class ATSBaseItem> GetHealingItemClass();
};
