// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TS_EnemyCharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AEnemyCharacter : public ATS_EnemyCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void BeginPlay() override;

	//virtual void TakeDamage(float DamageAmount) override;

	//void TakeHeadShot(int Damage);

	void AIOnDeath();



protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 CurrentHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	float Damage;

};
