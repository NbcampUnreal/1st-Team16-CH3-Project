// Fill out your copyright notice in the Description page of Project Settings.


#include "TSItemSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


ATSItemSpawnVolume::ATSItemSpawnVolume()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

    ItemDataTable = nullptr;
}

//랜덤한 아이템을 스폰시키는 함수
AActor* ATSItemSpawnVolume::SpawnRandomItem()
{
	if (FTSItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

//랜덤한 행을 가져오는 함수
FTSItemSpawnRow* ATSItemSpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr; //유효하지 않으면 null값 반환

	TArray<FTSItemSpawnRow*> AllRows; //모든 행을 가져올 배열
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	//전체 확률의 합 구하기
	float TotalChance = 0.0f;
	for (const FTSItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->Spawnchance;
		}
	}

	//총합과 0 사이의 랜덤 값 뽑기
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f; //누적확률 초기화

	for (FTSItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->Spawnchance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;

	// 누적확률 방식의 랜덤 뽑기
}


// 랜덤 위치 계산
FVector ATSItemSpawnVolume::GetRandomPointInVolume() const
{
	//Box의 크기 정도(Extent)를 벡터로 가져오는 함수
	//박스 컴포넌트의 스케일된 Extent, 즉 x/y/z 방향으로 반지름(절반 길이)을 구함
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();

	// 중심 구하는 함수
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	// 중심값(BoxOrigin)으로 부터 좌우 = 전체 길이값
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

//랜덤한 아이템 스폰시키는 함수
AActor* ATSItemSpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	// 안전 코드 ItemClass가 null값이면 리턴
	// 또는 최대 개수 초과 시 스폰 중단
	if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);

	return SpawnedActor;
}