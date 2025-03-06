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

	Tags.Add(TEXT("Player"));
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

	DefaultFOV = CameraComp->FieldOfView;
	DefaultCameraOffset = SpringArmComp->SocketOffset; // 카메라 컴포넌트 기본 위치를 저장한다.

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

	ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->SetShotGunBulletCount(CurrentShotGunBullet); //GameState로 데이터 전송
	}

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

	AActor* ChildActor = WeaponChildActor->GetChildActor();
	if (!ChildActor)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponChildActor->GetChildActor()가 nullptr입니다! 블루프린트에서 설정되었는지 확인하세요."));
		return;
	}

	AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChildActor에서 무기 장착 성공: %s"), *EquippedWeapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildActor를 AGunWeapon으로 캐스팅 실패! BP_ShotGun 또는 BP_M16이 AGunWeapon을 상속받았는지 확인하세요."));
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

	// 이동 사운드 재생
	PlayFootstepSound();
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

	// 현재 컨트롤러 회전값
	FRotator ControlRotation = Controller->GetControlRotation();

	// IMC에서 IA_Look에 Negate에 Y축 반전을 꺼도됐지만
	// 보기편하게 C++에 ControlRotation에 Pitch값에 ' - '를 넣어서 축반전을 넣음
	float NewPitch = ControlRotation.Pitch - LookInput.Y;

	// Pitch 각도를 -30 ~ 40도로 제한
	NewPitch = FMath::Clamp(NewPitch, MaxLookDownAngle, MaxLookUpAngle);

	// 제한된 Pitch값을 적용하고, Yaw값은 그대로두어 Pitch값에 Max치만 적용함
	Controller->SetControlRotation(FRotator(NewPitch, ControlRotation.Yaw + LookInput.X, 0.0f));
}

void ATSCharacter2::StartSprint(const FInputActionValue& value)
{
	// 재장전 중이면 Sprint 불가하게 만듬
	if (!GetCharacterMovement() || bIsReloading) return;

	// 조준 중이면 StopAiming으로 조준 해제
	if (bIsAiming)
	{
		StopAiming(value);
	}

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

	// 조준과 장전중이 아니면 기본 속도로 돌아간다.
	if (!bIsReloading && !bIsAiming) 
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
	if (GetCharacterMovement()->IsFalling() || !bIsAiming) // 점프나 조준중이 아니면 발사 금지
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

		ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GameState)
		{
			GameState->SetShotGunBulletCount(CurrentShotGunBullet); //GameState로 데이터 전송
		}
		
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

		// 총 발사 사운드 실행
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// 총쏘는 애니메이션 0.3초 설정 -> 1초로하면 왼손이 재장전하는데 샷건 재장전 메쉬를 움직일수가없음
		GetWorld()->GetTimerManager().SetTimer(
			ShotgunCooldownTimerHandle,
			this,
			&ATSCharacter2::ResetFireState,
			0.3f,
			false
		);
	}
}

void ATSCharacter2::StartAiming(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (GetCharacterMovement()->MaxWalkSpeed == SprintSpeed) return;

	bIsAiming = true;

	// 카메라 시야각 적용
	CameraComp->SetFieldOfView(AimFOV);

	// 카메라의 위치
	SpringArmComp->SocketOffset = FVector(200.0f, -50.0f, -20.0f);

	// 카메라 각도를 약간 아래로 기울여 조준 시 총이 중앙으로 오도록 조정
	SpringArmComp->SetRelativeRotation(FRotator(-8.0f, 5.0f, 0.0f));

	// 캐릭터의 조준 방향을 마우스 커서 방향으로 회전
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 카메라의 방향을 따라 정렬하도록 설정
		FRotator NewAimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraLocation + CameraRotation.Vector() * 1000.0f);
		NewAimRotation.Pitch = 0.0f; // 피치는 고정하여 위아래 각도 영향을 최소화
		NewAimRotation.Roll = 0.0f;
		SetActorRotation(NewAimRotation);
	}
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

	ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->SetShotGunBulletCount(CurrentShotGunBullet); //GameState로 데이터 전송
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


void ATSCharacter2::PlayFootstepSound()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (!GetCharacterMovement() || !bCanPlayFootstep) return;

	float CurrentSpeed = GetCharacterMovement()->Velocity.Size();

	// 속도가 10 이하이면 정지 상태로 판단하고 재생 X
	if (CurrentSpeed <= 10.0f)
	{
		return;
	}

	USoundCue* FootstepSound = nullptr;

	// 속도에 따라 걷는 소리 또는 뛰는 소리 선택
	if (CurrentSpeed > 300.0f)
	{
		FootstepSound = SprintSound; // 뛰는 사운드
	}
	else
	{
		FootstepSound = WalkSound; // 걷는 사운드
	}

	// 사운드가 존재하면 실행
	if (FootstepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, GetActorLocation());

		// 🔹 다음 발소리가 일정 시간 후에만 재생되도록 타이머 설정
		bCanPlayFootstep = false;
		float FootstepDelay = (CurrentSpeed > 300.0f) ? 0.23f : 0.5f; // 뛰는 경우 0.3초, 걷는 경우 0.5초
		GetWorld()->GetTimerManager().SetTimer(
			FootstepTimerHandle,
			this,
			&ATSCharacter2::ResetFootStep,
			FootstepDelay,
			false);
	}
}

void ATSCharacter2::ResetFootStep()
{
	bCanPlayFootstep = true;
}