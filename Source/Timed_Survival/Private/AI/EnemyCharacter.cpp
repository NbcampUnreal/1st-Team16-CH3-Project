// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentHP = 100;
	MaxHP = 100;
	Damage = 10.f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 120.f;
	}
}

//void AEnemyCharacter::TakeDamage(int32 Damage)
//{
//	if (CurrentHP > 0)
//	{
//		CurrentHP = FMath::Clamp(CurrentHP - Damage, 0, MaxHP);
//	}
//}
//
//void AEnemyCharacter::TakeHeadShot(int32 Damage)
//{
//	// 데미지값을 어디서 할지 고민좀 해보겠습니다..
//}

void AEnemyCharacter::AIOnDeath()
{
	Destroy();
}
