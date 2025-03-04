// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TS_EnemyCharacterBase.h"
#include "EnemyCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

class UAnimMontage;

UCLASS()
class TIMED_SURVIVAL_API AEnemyCharacter : public ATS_EnemyCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;

public:
	AEnemyCharacter();
	
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void AIOnDeath();

protected:
	virtual void BeginAttack();
	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

public:
	bool bIsNowAttacking;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 CurrentHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	float Damage;

};
