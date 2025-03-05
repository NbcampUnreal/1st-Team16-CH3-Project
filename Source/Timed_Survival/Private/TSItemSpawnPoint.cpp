#include "TSItemSpawnPoint.h"
#include "TSBaseItem.h"
#include "TSMineItem.h"
#include "TSMaskItem.h"
#include "TSBigHealingItem.h"
#include "TSMiddleHealingItem.h"
#include "TSSmallHealingItem.h"
#include "Kismet/GameplayStatics.h"

ATSItemSpawnPoint::ATSItemSpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
    SpawnProbability = 100.0f;
    ItemType = ETSItemType::Healing;

    // 기본 확률값 (100%가 아니어도 동작)
    BigHealingProbability = 25.0f;
    MiddleHealingProbability = 35.0f;
    SmallHealingProbability = 40.0f;
}

// **아이템을 스폰하는 함수**
void ATSItemSpawnPoint::SpawnItem()
{
    TSubclassOf<ATSBaseItem> ItemClass = GetItemClass();

    if (!ItemClass || FMath::RandRange(0.0f, 100.0f) > SpawnProbability)
    {
        return;
    }

    // **아이템 스폰**
    SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, GetActorLocation(), GetActorRotation());

    // **스폰된 아이템이 TSBaseItem을 상속받는다면 아웃라인 활성화**
    ATSBaseItem* BaseItem = Cast<ATSBaseItem>(SpawnedItem);
    if (BaseItem && ItemType != ETSItemType::Mine)  // 지뢰는 예외 처리
    {
        BaseItem->EnableOutline(true);
    }
}

// **아이템 클래스를 결정하는 함수**
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

// **힐링 아이템 확률 조정 (블루프린트 설정 유지)**
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