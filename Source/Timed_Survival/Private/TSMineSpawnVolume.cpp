// Fill out your copyright notice in the Description page of Project Settings.


#include "TSMineSpawnVolume.h"


ATSMineSpawnVolume::ATSMineSpawnVolume()
{
}

void ATSMineSpawnVolume::BeginPlay()
{
    Super::BeginPlay();

    if (!ItemDataTable) return;

    TArray<FTSItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("MineSpawnContext"));
    ItemDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return;

    int32 MaxSpawnCount = 10;
    int32 SpawnCount = FMath::RandRange(0, MaxSpawnCount); // 0~10개 랜덤 스폰

    for (int32 i = 0; i < SpawnCount; i++)
    {
        float TotalChance = 0.0f;
        for (const FTSItemSpawnRow* Row : AllRows)
        {
            if (Row) TotalChance += Row->Spawnchance;
        }

        float RandValue = FMath::FRandRange(0.0f, TotalChance);
        float AccumulateChance = 0.0f;

        for (FTSItemSpawnRow* Row : AllRows)
        {
            AccumulateChance += Row->Spawnchance;
            if (RandValue <= AccumulateChance)
            {
                SpawnItems(Row->ItemClass.Get());
                break;
            }
        }
    }
}
