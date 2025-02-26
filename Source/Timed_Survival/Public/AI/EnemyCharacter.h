// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	void TakeDamage(int Damage);

	void TakeHeadShot(int Damage);

	void AIOnDeath();



protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int CurrentHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int MaxHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	float Damage;

};
