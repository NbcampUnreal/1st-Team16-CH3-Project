// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "AI/Animation/TSEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TSPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TSGameState.h"
#include "Engine/TimerHandle.h"


AEnemyCharacter::AEnemyCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentHP = 100;
	MaxHP = 100;
	Damage = 10.f;

	OverheadHPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadHPBar"));
	OverheadHPBar->SetupAttachment(GetMesh());
	OverheadHPBar->SetWidgetSpace(EWidgetSpace::Screen);

	AttackRange = 40.f;

	BeforeTakeDamage = MaxHP;
	AfterTakeDamage = MaxHP;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	UpdateOverheadHP();

	GetWorldTimerManager().SetTimer( //Overhead Timer
		UpdateHPBarTimerHandle,
		this,
		&AEnemyCharacter::UpdateOverheadHP,
		0.1f, true);

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
		BeforeTakeDamage = AfterTakeDamage;
		CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, 100);
		AfterTakeDamage = CurrentHP;

		if (CurrentHP <= 0)
		{
			AIOnDeath();
		}
	}
	return DamageAmount;
}

void AEnemyCharacter::OnCheckHit()
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (IsValid(AIController) == true)
    {
        ACharacter* Player = Cast<ACharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey));
        if (IsValid(Player) == true)
        {
			ATSGameState* GameState = Cast<ATSGameState>(GetWorld()->GetGameState());
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
				if (IsValid(GameState))
				{
					GameState->ReduceTime(Damage,true);
				}
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

void AEnemyCharacter::DestroyedAI()
{
	Destroy();
}

void AEnemyCharacter::BeginAttack()
{
	UTSEnemyAnimInstance* AnimInstance = Cast<UTSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	if (CurrentHP > 0)
	{

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

void AEnemyCharacter::AIOnDeath()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UTSEnemyAnimInstance* AnimInstance = Cast<UTSEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (true == ::IsValid(AIController))
	{
		AIController->EndAI();
	}

	if (IsValid(AnimInstance) == true && IsValid(DeathMontage) == true && AnimInstance->Montage_IsPlaying(DeathMontage) == false)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(TEXT("DeathMontage"), DeathMontage);
	}


	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("AIDeath")));
	GetWorldTimerManager().SetTimer(
		DeathTimer,
		this,
		&AEnemyCharacter::DestroyedAI,
		2.5f,
		false
	);
}

//about OverHead UI

void AEnemyCharacter::UpdateOverheadHP()
{
	if (!OverheadHPBar) return;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			UUserWidget* ShotEventWidgetInstance = OverheadHPBar->GetUserWidgetObject();
			if (!ShotEventWidgetInstance) return;

			//1) HP Bar
			if (UProgressBar* HPBar = Cast<UProgressBar>(ShotEventWidgetInstance->GetWidgetFromName(TEXT("AI_HPBar"))))
			{
				HPBar->SetPercent(CurrentHP / MaxHP);
				OverheadHPBar->SetTranslucentSortPriority(-1);

				ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
				APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(Player, 0);				
				FVector CameraLocation = PlayerCamera->GetCameraLocation();

				FVector HPBarLocation = OverheadHPBar->GetComponentLocation();

				FRotator HPBarView = UKismetMathLibrary::FindLookAtRotation(HPBarLocation, CameraLocation);
				OverheadHPBar->SetWorldRotation(HPBarView);
				SetActorRotation(HPBarView);
			}

			//2)Damage Num
			if (UTextBlock* DamageNum = Cast<UTextBlock>(ShotEventWidgetInstance->GetWidgetFromName(TEXT("Damage"))))
			{
				float DamageValue = BeforeTakeDamage - AfterTakeDamage;
				int32 Damageint32 = FMath::RoundToInt(DamageValue);
				DamageNum->SetText(FText::FromString(FString::Printf(TEXT("%d"), Damageint32)));
			}
		}
	}
}

