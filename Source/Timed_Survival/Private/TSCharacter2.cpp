#include "TSCharacter2.h"
#include "TSGameState.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TSPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

ATSCharacter2::ATSCharacter2()
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

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;

}

// 무기 타입으로 무기 찾는 함수(총알 추가용)
AGunWeapon* ATSCharacter2::FindWeaponByType(FName WeaponType)
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

void ATSCharacter2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&ATSCharacter2::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter2::StopJump
				);
			}


			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::Look
				);
			}


			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter2::StopSprint
				);
			}


			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::StartCrouch
				);

				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter2::StopCrouch
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::Reload
				);
			}

			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::Fire
				);
			}

			if (PlayerController->AimingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter2::StartAiming
				);
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter2::StopAiming
				);
			}
		}
	}
}

void ATSCharacter2::BeginPlay()
{
	Super::BeginPlay();

	// 기본적으로 총을 안쏘는 상태로 시작하게 false로 설정
	bFire = false;

	// 기본적으로 장전하는 애니메이션이 안나오도록 false로 설정
	bIsReloading = false;

	MaxShotGunBullet = 2;
	CurrentShotGunBullet = MaxShotGunBullet;

	if (CurrentShotGunBullet <= 0)
	{
		CurrentShotGunBullet = 0;
	}
	else if (CurrentShotGunBullet >= 2)
	{
		CurrentShotGunBullet = 2;
	}

	DefaultFOV = CameraComp->FieldOfView;
	DefaultCameraOffset = SpringArmComp->SocketOffset; // 카메라 컴포넌트 기본 위치를 저장한다.

	//테스트

	if (!WeaponChildActor)
	{
		WeaponChildActor = FindComponentByClass<UChildActorComponent>();
	}

	if (!WeaponChildActor)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponChildActor가 nullptr입니다! 블루프린트에서 설정되었는지 확인하세요."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("WeaponChildActor가 정상적으로 설정되었습니다."));

	AActor* ChildActor = WeaponChildActor->GetChildActor();
	if (!ChildActor)
	{
		UE_LOG(LogTemp, Error, TEXT(" WeaponChildActor->GetChildActor()가 nullptr입니다! BP_M16이 제대로 설정되었는지 확인하세요."));
		return;
	}

	AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChildActor에서 무기 장착 성공: %s"), *EquippedWeapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildActor를 AGunWeapon으로 캐스팅 실패! BP_M16이 AGunWeapon을 상속받았는지 확인하세요."));
	}
}

void ATSCharacter2::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	FaceMouseDirection();

	if (GetCharacterMovement()->MaxWalkSpeed == SprintSpeed)
	{
		if (LastMoveInput.X <= 0.0f || !FMath::IsNearlyZero(LastMoveInput.Y))
		{
			StopSprint(FInputActionValue());
		}
	}

	if (!IsFiring && LastMoveInput.IsNearlyZero() && !LastMoveDirection.IsNearlyZero())
	{
		AddMovementInput(LastMoveDirection, 1.0f);
	}
}

void ATSCharacter2::Move(const FInputActionValue& value)
{
	if (!Controller || bIsReloading) return; // 재장전 중이면 이동 불가

	FVector2D MoveInput = value.Get<FVector2D>();
	if (FMath::IsNearlyZero(MoveInput.X) && FMath::IsNearlyZero(MoveInput.Y))
	{
		return;
	}

	LastMoveInput = MoveInput;

	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0, ControlRotation.Yaw, 0);

	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveDirection = (Forward * MoveInput.X) + (Right * MoveInput.Y);
	MoveDirection = MoveDirection.GetSafeNormal();

	IsMovingForward = (MoveInput.X > 0.0f && FMath::IsNearlyZero(MoveInput.Y));

	LastMoveDirection = MoveDirection;

	SetActorRotation(YawRotation);
	AddMovementInput(MoveDirection, 1.0f);
}

void ATSCharacter2::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ATSCharacter2::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ATSCharacter2::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();


	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ATSCharacter2::StartSprint(const FInputActionValue& value)
{
	// 재장전 중이면 Sprint 불가하게 만듬
	if (!GetCharacterMovement() || bIsReloading) return;

	if (LastMoveInput.X > 0.0f && FMath::IsNearlyZero(LastMoveInput.Y))
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}


void ATSCharacter2::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	if (!bIsReloading)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ATSCharacter2::StartCrouch(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	Crouch();
}

void ATSCharacter2::StopCrouch(const FInputActionValue& value)
{
	UnCrouch();
}

void ATSCharacter2::Reload(const FInputActionValue& value)
{
	if (bIsReloading || GetCharacterMovement()->IsFalling()) // 연속 입력 방지 및 점프 중 재장전 금지
	{
		return;
	}

	if (CurrentShotGunBullet < MaxShotGunBullet)
	{
		bIsReloading = true; // 재장전 중 상태 설정

		GetCharacterMovement()->DisableMovement(); // 이동 완전 차단

		// 재장전 쿨다운 3초
		GetWorld()->GetTimerManager().SetTimer(
			ShotgunCooldownTimerHandle,
			this,
			&ATSCharacter2::ResetReloadState,
			3.0f,
			false
		);

	}
}

void ATSCharacter2::Fire(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling()) // 점프 중에는 발사 금지
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	if (!bFire && CurrentShotGunBullet > 0) // 연속 발사 방지 및 총알이 있을 때만 실행
	{
		bFire = true; // 발사 중 상태 설정

		// 탄약 감소
		CurrentShotGunBullet = FMath::Clamp(CurrentShotGunBullet - 1, 0, MaxShotGunBullet);

		// Test출력용
		// ================================================================================
		// ================================================================================
		UE_LOG(LogTemp, Warning, TEXT("총알 발사 현재 남은 총알 : %d"), CurrentShotGunBullet);

		// 무기 발사 처리
		if (WeaponChildActor)
		{
			AActor* ChildActor = WeaponChildActor->GetChildActor();
			if (ChildActor)
			{
				AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
				if (EquippedWeapon)
				{
					EquippedWeapon->FireBullet();
				}
			}
		}

		// 0.2초 후 다시 발사 가능하도록 설정
		GetWorld()->GetTimerManager().SetTimer(
			ShotgunCooldownTimerHandle,
			this,
			&ATSCharacter2::ResetFireState,
			0.2f,
			false
		);
	}
}



void ATSCharacter2::StartAiming(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (GetCharacterMovement()->MaxWalkSpeed == 1000)
	{
		return;
	}

	bIsAiming = true;
	CameraComp->SetFieldOfView(AimFOV);
	SpringArmComp->SocketOffset = FVector(260, -40, -54);
	SpringArmComp->SetRelativeRotation(FRotator(0, -4, 0));
}

void ATSCharacter2::StopAiming(const FInputActionValue& value)
{
	bIsAiming = false;
	CameraComp->SetFieldOfView(DefaultFOV);
	SpringArmComp->SocketOffset = DefaultCameraOffset;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}



void ATSCharacter2::Death()
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


// About Health
void ATSCharacter2::TakeDamage()
{

}

void ATSCharacter2::EnableMovementAfterReload()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ATSCharacter2::FaceMouseDirection()
{
	if (!Controller) return;

	FRotator NewRotation = Controller->GetControlRotation();
	NewRotation.Pitch = 0.0f;
	NewRotation.Roll = 0.0f;

	SetActorRotation(NewRotation);
}

void ATSCharacter2::ResetMovementAfterFire()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	IsFiring = false;
}
void ATSCharacter2::ResetFireState()
{
	bFire = false; // Fire 상태 해제
}

void ATSCharacter2::ResetReloadState()
{
	bIsReloading = false; // 재장전 상태 해제

	GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 이동 가능하게 복구
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;


	// 탄약 증가 로직
	if (CurrentShotGunBullet == 0)
	{
		CurrentShotGunBullet = 2; // 0이면 2로 증가
	}
	else if (CurrentShotGunBullet == 1)
	{
		CurrentShotGunBullet = 2; // 1이면 2로 증가
	}

	// Test출력용
	// ================================================================================
	// ================================================================================
	UE_LOG(LogTemp, Warning, TEXT("샷건 장전 현재 총알 : %d"), CurrentShotGunBullet);

}

int32 ATSCharacter2::GetCurrentShotGunBullet() const
{
	return CurrentShotGunBullet;
}
