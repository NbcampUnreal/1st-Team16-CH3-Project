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

	Tags.Add(TEXT("Player"));
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

	// 이동 사운드 재생
	PlayFootstepSound();
}

void ATSCharacter::Move(const FInputActionValue& value)
{
	if (!Controller || bFire)
	{
		return;
	}
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

	// 현재 컨트롤러 회전값
	FRotator ControlRotation = Controller->GetControlRotation();

	// IMC에서 IA_Look에 Negate에 Y축 반전을 꺼도됐지만
	// 보기편하게 C++에 ControlRotation에 Pitch값에 ' - '를 넣어서 축반전을 넣음
	float NewPitch = ControlRotation.Pitch - LookInput.Y;

	// Pitch 각도를 -30 ~ 30도로 제한
	NewPitch = FMath::Clamp(NewPitch, MaxLookDownAngle, MaxLookUpAngle);

	// 제한된 Pitch값을 적용하고, Yaw값은 그대로두어 Pitch값에 Max치만 적용함
	Controller->SetControlRotation(FRotator(NewPitch, ControlRotation.Yaw + LookInput.X, 0.0f));
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
	if (GetCharacterMovement()->IsFalling()) return;

	// 이미 재장전 중이면 return
	if (bIsReloading) return;

	if (bFire)
	{
		StopFire(value);
	}

	bIsReloading = true;

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
	// 떨어지고있거단 조준중이아니면 return;
	if (GetCharacterMovement()->IsFalling() || !bIsAiming) return;
	// 장전중이면 return;
	if (bIsReloading) return;

	// ChildActor가없으면 return;
	if (!WeaponChildActor)
	{
		return;
	}

	AActor* ChildActor = WeaponChildActor->GetChildActor();
	if (!ChildActor)
	{
		return;
	}

	AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
	if (!EquippedWeapon)
	{
		return;
	}

	// 현재 탄약 개수 저장할 변수
	int32 PreviousBulletCount = CurrentBullet;

	// 현재 탄약 개수를 CurrentBullet에 업데이트
	CurrentBullet = EquippedWeapon->GetBulletCount();

	if (CurrentBullet <= 0)
	{
		bFire = false;
		return;
	}

	EquippedWeapon->StartFire();

	// 총 발사 후 탄약 개수 다시 가져오기 (변경 후)
	int32 NewBulletCount = EquippedWeapon->GetBulletCount();

	// 이전 탄약 개수보다 현재 탄약 개수가 감소했을 때만 사운드 실행
	if (NewBulletCount < PreviousBulletCount && FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// 발사상태 true;
	bFire = true;
}


void ATSCharacter::StopFire(const FInputActionValue& value)
{
	// 캐릭터 이동속도 원상 복귀
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	// GunWeapon에 StopFire()호출해서 타이머핸들 Clear해준다.
	AActor* ChildActor = WeaponChildActor->GetChildActor();
	AGunWeapon* EquippedWeapon = Cast<AGunWeapon>(ChildActor);
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFire();
	}

	// Fire 변수를 false로 설정하여 애니메이션 블루프린트에서 감지 가능하게 함
	bFire = false;
}


void ATSCharacter::StartAiming(const FInputActionValue& value)
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

void ATSCharacter::StopAiming(const FInputActionValue& value)
{
	bIsAiming = false;
	CameraComp->SetFieldOfView(DefaultFOV);
	SpringArmComp->SocketOffset = DefaultCameraOffset;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// 조준 중 발사할때 조준을 그만해도 발사하는 버그때문에 조준 해제시 발사중이면 StopFire함수 호출
	if (bFire)
	{
		StopFire(value);
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


// About Health
float ATSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if (CurrentHP > 0)
	//{
	//	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, 100);
	//	UE_LOG(LogTemp, Warning, TEXT("남은 체력 : %f"), CurrentHP);

	//	if (CurrentHP <= 0)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("사망"), CurrentHP);
	//		Death();
	//	}
	//}
	return ResultDamage;
}

void ATSCharacter::EnableMovementAfterReload()
{
	bIsReloading = false;
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

void ATSCharacter::PlayFootstepSound()
{
	if (!GetCharacterMovement()) return;

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
	}
}