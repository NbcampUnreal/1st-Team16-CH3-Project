#include "TSItemSpawnPoint.h"
#include "TSBaseItem.h"
#include "TSMineItem.h"
#include "TSMaskItem.h"
#include "TSBigHealingItem.h"
#include "TSMiddleHealingItem.h"
#include "TSSmallHealingItem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h" // 리스폰 사용시 타이머 사용을 위해 필요

ATSItemSpawnPoint::ATSItemSpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawnProbability = 100.0f;
    ItemType = ETSItemType::Healing;

    // 기본 확률값 (100%가 아니어도 동작)
    BigHealingProbability = 25.0f;
    MiddleHealingProbability = 35.0f;
    SmallHealingProbability = 40.0f;

    bCanRespawn = false; // 기본값: 리스폰 비활성화
}

// 아이템을 스폰하는 함수
void ATSItemSpawnPoint::SpawnItem()
{
    TSubclassOf<ATSBaseItem> ItemClass = GetItemClass();

    if (!ItemClass || FMath::RandRange(0.0f, 100.0f) > SpawnProbability)
    {
        return;
    }

    // 아이템 스폰
    SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, GetActorLocation(), GetActorRotation());

    // 스폰된 아이템이 TSBaseItem을 상속받는다면 아웃라인 활성화
    ATSBaseItem* BaseItem = Cast<ATSBaseItem>(SpawnedItem);
    if (BaseItem && ItemType != ETSItemType::Mine)  // 지뢰는 예외 처리
    {
        BaseItem->EnableOutline(true);
    }

	// 아이템을 일정시간 후 리스폰 하고싶다면 아래 코드 사용
    // 일정 시간이 지나면 RespawnItem() 실행
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ATSItemSpawnPoint::RespawnItem, 5.0f, false);
}

// 아이템 클래스를 결정하는 함수
TSubclassOf<ATSBaseItem> ATSItemSpawnPoint::GetItemClass()
{
    switch (ItemType)
    {
    case ETSItemType::Mine:
        return ATSMineItem::StaticClass();
    case ETSItemType::Healing:
        return GetHealingItemClass();
    case ETSItemType::Mask:
        return ATSMaskItem::StaticClass();
    default:
        return nullptr;
    }
}

// 힐링 아이템 확률 조정 (블루프린트 설정 유지)
TSubclassOf<ATSBaseItem> ATSItemSpawnPoint::GetHealingItemClass()
{
    float TotalProbability = BigHealingProbability + MiddleHealingProbability + SmallHealingProbability;

    if (TotalProbability <= 0.0f)
    {
        return nullptr;
    }

    float RandomValue = FMath::RandRange(0.0f, TotalProbability);

    if (RandomValue < BigHealingProbability && BigHealingBP)
    {
        return BigHealingBP;
    }
    else if (RandomValue < BigHealingProbability + MiddleHealingProbability && MiddleHealingBP)
    {
        return MiddleHealingBP;
    }
    else if (SmallHealingBP)
    {
        return SmallHealingBP;
    }

    return nullptr;
}


// 아래는 추가 구현
// 아이템 리스폰 함수 (아이템이 제거된 경우에만 다시 스폰)
void ATSItemSpawnPoint::RespawnItem()
{
    // 리스폰이 비활성화되었으면 실행 X
    if (!bCanRespawn)
    {
        return;
    }

    // 기존 아이템이 유효하고 삭제되지 않았다면 리스폰 중단
    if (IsValid(SpawnedItem) && !SpawnedItem->IsActorBeingDestroyed())
    {
        return;
    }

    // 기존 아이템이 제거된 경우에만 다시 아이템 스폰
    SpawnedItem = nullptr;
    SpawnItem();
}

// 리스폰 활성화 함수
void ATSItemSpawnPoint::EnableRespawn()
{
    bCanRespawn = true;
}

// 리스폰 비활성화 함수
void ATSItemSpawnPoint::DisableRespawn()
{
    bCanRespawn = false;

    // 현재 실행 중인 리스폰 타이머 제거
    GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}