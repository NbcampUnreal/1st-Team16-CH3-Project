#include "TSCharacter.h"
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


			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter::StartCrouch
				);

				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter::StopCrouch
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
					&ATSCharacter::StartFire
				);
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter::StopFire
				);
			}

			if (PlayerController->AimingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Triggered,
					this,
					&ATSCharacter::StartAiming
				);
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Completed,
					this,
					&ATSCharacter::StopAiming
				);
			}
		}
	}
}

void ATSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 기본적으로 총을 안쏘는 상태로 시작하게 false로 설정
	bFire = false;

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

void ATSCharacter::Tick(float DeltaTime)
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

void ATSCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
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

void ATSCharacter::StartJump(const FInputActionValue& value)
{
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
	if (!GetCharacterMovement()) return;

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


void ATSCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ATSCharacter::StartCrouch(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	Crouch();
}

void ATSCharacter::StopCrouch(const FInputActionValue& value)
{
	UnCrouch();
}


void ATSCharacter::Reload(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true && IsValid(ReloadAnimation) == true && AnimInstance->Montage_IsPlaying(ReloadAnimation) == false)
	{
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

		//Gunweapon 함수 호출
		if (WeaponChildActor)
		{
			AActor* ChildActor = WeaponChildActor->GetChildActor();
			AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
			if (EquippedWeapon)
			{
				EquippedWeapon->Reload();
			}
		}
	}
}

void ATSCharacter::StartFire(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling()) // 점프 중에는 발사 금지
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	//테스트
	if (!WeaponChildActor)
	{
		UE_LOG(LogTemp, Error, TEXT(" Fire(): WeaponChildActor가 nullptr입니다! 블루프린트에서 설정되었는지 확인하세요."));
		return;
	}

	AActor* ChildActor = WeaponChildActor->GetChildActor();
	if (!ChildActor)
	{
		UE_LOG(LogTemp, Error, TEXT(" Fire(): WeaponChildActor->GetChildActor()가 nullptr입니다! BP_M16이 올바르게 설정되었는지 확인하세요."));
		return;
	}

	//  GunWeapon 타입으로 캐스팅하여 FireBullet() 호출
	AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Fire(): 무기 발사 시도!"));

		//  FireBullet() 실행 전 확인 로그 추가
		if (EquippedWeapon->GetBulletCount() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire(): 탄약 개수 충분 - FireBullet() 실행!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(" Fire(): 탄약 없음! FireBullet() 실행 불가!"));
		}

		EquippedWeapon->FireBullet();
		UE_LOG(LogTemp, Warning, TEXT(" Fire(): FireBullet() 호출 완료!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Fire(): ChildActor를 AGunWeapon으로 캐스팅 실패! BP_M16이 GunWeapon을 상속받았는지 확인하세요."));
	}

	// Fire 변수를 true로 설정하여 애니메이션 블루프린트에서 감지 가능하게 함
	bFire = true;
}

void ATSCharacter::StopFire(const FInputActionValue& value)
{
	// 캐릭터 이동속도 원상 복귀
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}


	// Fire 변수를 false로 설정하여 애니메이션 블루프린트에서 감지 가능하게 함
	bFire = false;
}


void ATSCharacter::StartAiming(const FInputActionValue& value)
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

void ATSCharacter::StopAiming(const FInputActionValue& value)
{
	bIsAiming = false;
	CameraComp->SetFieldOfView(DefaultFOV);
	SpringArmComp->SocketOffset = DefaultCameraOffset;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
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


// About Health
void ATSCharacter::TakeDamage()
{

}

void ATSCharacter::EnableMovementAfterReload()
{
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

void ATSCharacter::ResetMovementAfterFire()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	IsFiring = false;
}
void ATSCharacter::ResetFireState()
{
	bFire = false; // Fire 상태 해제
}