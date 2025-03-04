// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "AI/Animation/TSEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyCharacter::AEnemyCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentHP = 100;
	MaxHP = 100;
	Damage = 10.f;
	AttackRange = 40.f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UTSEnemyAnimInstance* AnimInstance = Cast<UTSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true)
	{
		
	}

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

#include "DrawDebugHelpers.h"

// ...

void AEnemyCharacter::OnCheckHit()
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (IsValid(AIController) == true)
    {
        ACharacter* Player = Cast<ACharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey));
        if (IsValid(Player) == true)
        {
            FHitResult HitResult;
            FCollisionQueryParams Params(NAME_None, false, this);
            const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
            const FVector End = Start + GetActorForwardVector() * AttackRange;
            const float Radius = 50.f;
            // 콜리전 오버랩 체크
            bool bOnHit = GetWorld()->SweepSingleByChannel(
                HitResult,
                Start,
                End,
                FQuat::Identity,
                ECollisionChannel::ECC_GameTraceChannel2,
                FCollisionShape::MakeSphere(Radius),
                Params
            );

            // ApplyDamage() 호출
            if (bOnHit == true)
            {
				UKismetSystemLibrary::PrintString(this, TEXT("OnCheckHit()"));
                /*UGameplayStatics::ApplyDamage(
                    Player,
                    Damage,
                    GetInstigator()->GetController(),
                    this,
                    UDamageType::StaticClass()
                );*/
            }
            if (AEnemyAIController::ShowAIDebug == 1)
            {
                FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
                float CapsuleHalfHeight = AttackRange * 0.5f;

                DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, Radius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Red, false, 5.0f);
            }
        }
    }
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
