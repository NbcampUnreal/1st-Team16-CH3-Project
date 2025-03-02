// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSBaseBulletItem.h"
#include "TSARBulletItem.generated.h"



UCLASS()
class TIMED_SURVIVAL_API ATSARBulletItem : public ATSBaseBulletItem
{
	GENERATED_BODY()
	
public:
	ATSARBulletItem();

	virtual void ActivateItem(AActor* Activator) override;
};
