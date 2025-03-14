// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TS_EnemyCharacterBase.h"
#include "EnemyCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

struct FTimerHandle;
class UAnimMontage;
class UWidgetComponent;

UCLASS()
class TIMED_SURVIVAL_API AEnemyCharacter : public ATS_EnemyCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;

public:
	AEnemyCharacter();
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadDamageNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* OverheadHPBar;
	FTimerHandle UpdateHPBarTimerHandle;
	float BeforeTakeDamage;
	float AfterTakeDamage;

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void AIOnDeath();
	void UpdateOverheadHP();
	/*void SetOverheadWidgetViewFront();*/

	void OnCheckHit();
	void DestroyedAI();
protected:
	virtual void BeginAttack();
	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

public:
	bool bIsNowAttacking;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 CurrentHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	float Damage;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stats")
	float AttackRange;

};
