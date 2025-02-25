#include "TSCharacter.h"
#include "TSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATSCharacter::ATSCharacter() 
{
	PrimaryActorTick.bCanEverTick = false;

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
}


void ATSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInput 컴포넌트를 사용일때
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{   // 플레이어컨트롤러를 현재 컨트롤러일때
		if (ATSPlayerController* PlayerController = Cast<ATSPlayerController>(GetController()))
		{   // MoveInput이 설정 되어있으면 MoveAction이 플레이어컨트롤러
			if (PlayerController->MoveAction)
			{
				// MoveAction이 실행할때 AP2Character::Move함수가 실행
				EnhancedInput->BindAction(
					PlayerController->MoveAction, // 플레이어 컨트롤에있는 이동액션
					ETriggerEvent::Triggered,     // 트리거 이벤트
					this,                         // 실행 대상(W, A, S, D)
					&ATSCharacter::Move          // 실행 함수
				);
			}

			// JumpInput이 되어있으면 점프액션 호출
			if (PlayerController->JumpAction)
			{
				// 점프 시작(키) 시 AP2Character::StartJump가 호출
				EnhancedInput->BindAction(
					PlayerController->JumpAction, // 플레이어 컨트롤에있는 점프 액션
					ETriggerEvent::Triggered,     // 트리거 이벤트
					this,                         // 실행 대상(Space키)
					&ATSCharacter::StartJump     // 실행 함수
				);
				// 점프 종료(떨어질때) 시 AP2Character::StopJump가 호출
				EnhancedInput->BindAction(
					PlayerController->JumpAction, // 플레이어 컨트롤에있는 점프 액션
					ETriggerEvent::Completed,     // 트리거 이벤트
					this,                         // 실행 대상(Space키)
					&ATSCharacter::StopJump      // 실행 함수
				);
			}

			// 카메라를 마우스로 회전하는 설정이 되어있으면 look액션 호출
			if (PlayerController->LookAction)
			{
				// Look액션이 발생하면 PJ2Character::Look 함수 실행
				EnhancedInput->BindAction(
					PlayerController->LookAction, // 플레이어 컨트롤러에있는 마우스 회전 액션
					ETriggerEvent::Triggered,     // 트리거 이벤트
					this,                         // 실행 대상(마우스 회전)
					&ATSCharacter::Look          // 실행 함수
				);
			}

			// Sprint입력이 되어있으면 Sprint호출
			if (PlayerController->SprintAction)
			{
				// IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
				EnhancedInput->BindAction(
					PlayerController->SprintAction,// 플레이어 컨트롤에있는 달리기 액션
					ETriggerEvent::Triggered,      // 트리거 이벤트
					this,                          // 실행 대상(왼쪽 Shift키)
					&ATSCharacter::StartSprint    // 실행 함수
				);
				// IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
				EnhancedInput->BindAction(
					PlayerController->SprintAction,  // 플레이어 컨트롤에있는 달리기 액션
					ETriggerEvent::Completed,        // 트리거 이벤트
					this,                            // 실행 대상(왼쪽 Shift키)
					&ATSCharacter::StopSprint       // 실행 함수
				);
			}
		}
	}
}

void ATSCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; // 컨트롤러가 없으면 리턴
	const FVector2D MoveInput = value.Get<FVector2D>(); // MoveInput은 2D 벡터로 값을 들고옴

	// X축 입력이 있으면 캐릭터가 앞뒤으로 이동
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	// Y축 입력이 있으면 캐릭터가 좌우로 이동
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ATSCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>()) // 입력이 true면 점프함
	{
		Jump();
	}
}

void ATSCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>()) // 입력이 거짓이면 점프 멈춤
	{
		StopJumping();
	}
}

void ATSCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>(); // LookInput은 2D 벡터로 값을 들고옴

	// X축 입력으로 좌우 회전 (Yaw)
	AddControllerYawInput(LookInput.X);

	// Y축 입력으로 상하 회전 (Pitch)
	AddControllerPitchInput(LookInput.Y);
}

void ATSCharacter::StartSprint(const FInputActionValue& value)
{
	// Shift 키를 누른 순간 이 함수가 호출된다고 가정
	// 스프린트 속도를 적용
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATSCharacter::StopSprint(const FInputActionValue& value)
{
	// Shift 키를 뗀 순간 이 함수가 호출
	// 평상시 속도로 복귀
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}