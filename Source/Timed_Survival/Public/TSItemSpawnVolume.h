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
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	//데이터 테이블 가져오기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	//랜덤한 아이템을 스폰 시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();

	FTSItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass); //스폰함수 (템플릿 지정_AActor의 하위 클래스 포함)
	FVector GetRandomPointInVolume() const; // 랜덤 좌표

};
