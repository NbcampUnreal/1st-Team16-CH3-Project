#include "TSCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TSPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


ATSCharacter::ATSCharacter() 
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 300.0f;
	SprintSpeed = 1000.0f;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;
}

// 무기 타입으로 무기 찾는 함수(총알 추가용)
AGunWeapon* ATSCharacter::FindWeaponByType(FName WeaponType)
{
	for (AGunWeapon* Weapon : Weapons)
	{
		if (Weapon && Weapon->GetWeaponType() == WeaponType)
		{
			return Weapon; 
		}
	}
	return nullptr; 
}

void ATSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{   
		if (ATSPlayerController* PlayerController = Cast<ATSPlayerController>(GetController()))
		{  
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction, 
					ETriggerEvent::Triggered,     
					this,                         
					&ATSCharacter::Move        
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction, 
					ETriggerEvent::Triggered,   
					this,                       
					&ATSCharacter::StartJump     
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction, 
					ETriggerEvent::Completed,     
					this,                        
					&ATSCharacter::StopJump     
				);
			}


			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction, 
					ETriggerEvent::Triggered,    
					this,                       
					&ATSCharacter::Look         
				);
			}


			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,    
					this,                        
					&ATSCharacter::StartSprint   
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,  
					ETriggerEvent::Completed,       
					this,                           
					&ATSCharacter::StopSprint      
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter::Reload
				);
			}

			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter::Fire
				);
			}
		}
	}
}

void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAimOffset();
	FaceMouseDirection();
}


void ATSCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (FMath::IsNearlyZero(MoveInput.X) && FMath::IsNearlyZero(MoveInput.Y))
	{
		return;
	}

	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0, ControlRotation.Yaw, 0);

	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveDirection = (Forward * MoveInput.X) + (Right * MoveInput.Y);
	MoveDirection = MoveDirection.GetSafeNormal();

	FRotator TargetRotation = MoveDirection.Rotation();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
	SetActorRotation(NewRotation);

	AddMovementInput(MoveDirection, 1.0f);
}

void ATSCharacter::StartJump(const FInputActionValue& value)
{
	// firing and reloading prevent jump
	if (IsFiring || IsReloading)
	{
		return;
	}

	if (value.Get<bool>()) 
	{
		Jump();
	}
}

void ATSCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) 
	{
		StopJumping();
	}
}

void ATSCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();


	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ATSCharacter::StartSprint(const FInputActionValue& value)
{


	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATSCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ATSCharacter::Reload(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (IsFiring)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && IsValid(ReloadAnimation) && !AnimInstance->Montage_IsPlaying(ReloadAnimation))
	{
		IsReloading = true;
		GetCharacterMovement()->DisableMovement();
		AnimInstance->Montage_Play(ReloadAnimation);

		float ReloadTime = ReloadAnimation->GetPlayLength();
		GetWorld()->GetTimerManager().SetTimer(
			ReloadTimerHandle,
			this,
			&ATSCharacter::EnableMovementAfterReload,
			ReloadTime,
			false
		);
	}
}

void ATSCharacter::Fire(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (IsReloading)
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && IsValid(FireAnimation) && !AnimInstance->Montage_IsPlaying(FireAnimation))
	{
		IsFiring = true;
		AnimInstance->Montage_Play(FireAnimation);

		float FireTime = FireAnimation->GetPlayLength();
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&ATSCharacter::ResetFireState,
			FireTime,
			false
		);
	}
}

void ATSCharacter::Death()
{
	if (DeathAnimation)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FName SlotName = TEXT("DeathSlot");
			AnimInstance->Montage_Play(DeathAnimation);
			AnimInstance->Montage_JumpToSection(SlotName, DeathAnimation);
		}
	}

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
}

void ATSCharacter::UpdateAimOffset()
{
	if (!Controller) return;

	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator ActorRotation = GetActorRotation();

	AimRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);

}

void ATSCharacter::ResetFireState()
{
	IsFiring = false;
}

void ATSCharacter::EnableMovementAfterReload()
{
	IsReloading = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ATSCharacter::FaceMouseDirection()
{
	if (!Controller) return;

	FRotator NewRotation = Controller->GetControlRotation();
	NewRotation.Pitch = 0.0f;
	NewRotation.Roll = 0.0f;

	SetActorRotation(NewRotation);
}


