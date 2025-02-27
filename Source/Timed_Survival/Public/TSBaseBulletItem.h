// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSBaseItem.h"
#include "TSBaseBulletItem.generated.h"

/**
 * 
 */
UCLASS()
class TIMED_SURVIVAL_API ATSBaseBulletItem : public ATSBaseItem
{
	GENERATED_BODY()

public:
	ATSBaseBulletItem();

protected:

	// 총알 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	int32 BulletAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	FName WeaponType; 


	virtual void ActivateItem(AActor* Activator) override;
};

