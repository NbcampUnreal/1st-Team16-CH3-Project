// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "TSItemSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TIMED_SURVIVAL_API ATSItemSpawnVolume : public AActor
{
	GENERATED_BODY()
	

public:
	ATSItemSpawnVolume();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable; // 스폰할 데이터 테이블

	// 자식 클래스에서 구현할 함수 (BeginPlay에서 각각 호출)
	virtual void BeginPlay() override;

	// 아이템을 스폰하는 기본 함수 (공통 사용)
	AActor* SpawnItems(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;
};
