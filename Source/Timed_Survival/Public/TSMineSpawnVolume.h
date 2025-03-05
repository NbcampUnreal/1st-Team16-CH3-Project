// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSItemSpawnVolume.h"
#include "TSMineSpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSMineSpawnVolume : public ATSItemSpawnVolume
{
	GENERATED_BODY()
	
public:
	ATSMineSpawnVolume();

protected:
	virtual void BeginPlay() override; // BeginPlay에서 지뢰 0~10개 랜덤 스폰
};
