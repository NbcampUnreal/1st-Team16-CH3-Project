// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "AI/Animation/TSEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TSPlayerController.h"

AEnemyCharacter::AEnemyCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentHP = 100;
	MaxHP = 100;
	Damage = 10.f;

	OverheadHPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadHPBar"));
	OverheadHPBar->SetupAttachment(GetMesh());
	OverheadHPBar->SetWidgetSpace(EWidgetSpace::Screen);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateOverheadHP();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 120.f;
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (CurrentHP > 0)
	{
		CurrentHP -= FMath::Clamp(CurrentHP - DamageAmount, 0, 100);
	}
	else if (CurrentHP <= 0)
	{
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (true == ::IsValid(AIController))
		{
			AIController->EndAI();
		}
		AIOnDeath();
	}
	return DamageAmount;
}

void AEnemyCharacter::AIOnDeath()
{
	Destroy();
}

void AEnemyCharacter::BeginAttack()
{
	UTSEnemyAnimInstance* AnimInstance = Cast<UTSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMontage);

		bIsNowAttacking = true;

		if (OnAttackMontageEndedDelegate.IsBound() == false)
		{
			OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
		}
	}
}

void AEnemyCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsNowAttacking = false;

	if (OnAttackMontageEndedDelegate.IsBound() == true)
	{
		OnAttackMontageEndedDelegate.Unbind();
	}
}

void AEnemyCharacter::UpdateOverheadHP()
{
	if (!OverheadHPBar) return;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			UUserWidget* ShotEventWidgetInstance = OverheadHPBar->GetUserWidgetObject();
			if (!ShotEventWidgetInstance) return;
			if (UProgressBar* HPBar = Cast<UProgressBar>(ShotEventWidgetInstance->GetWidgetFromName(TEXT("AI_HPBar"))))
			{
				
				HPBar->SetPercent(CurrentHP / MaxHP);
			}
		}
	}
	
}
