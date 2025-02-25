#include "TSCharacter.h"
#include "TSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		}
	}
}

void ATSCharacter::Move(const FInputActionValue& value)
{
<<<<<<< Updated upstream
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
=======
    if (!Controller) return; 
    const FVector2D MoveInput = value.Get<FVector2D>();

    // 입력이 없으면 아무것도 하지 않음
    if (FMath::IsNearlyZero(MoveInput.X) && FMath::IsNearlyZero(MoveInput.Y))
    {
        return;
    }

    // ✅ 현재 컨트롤러(카메라)의 방향을 기준으로 이동 방향 계산
    FRotator ControlRotation = Controller->GetControlRotation();
    FRotator YawRotation(0, ControlRotation.Yaw, 0); // 피치, 롤 제거 (수평 회전만 사용)

    FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // 카메라 앞 방향
    FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);   // 카메라 오른쪽 방향

    // ✅ 입력 방향을 카메라 기준으로 변환
    FVector MoveDirection = (Forward * MoveInput.Y) + (Right * MoveInput.X);
    MoveDirection = MoveDirection.GetSafeNormal(); // 정규화

    // ✅ 목표 회전값 설정 (입력한 방향을 바라보도록)
    FRotator TargetRotation = MoveDirection.Rotation();
    TargetRotation.Pitch = 0.0f; // 상하 회전 방지
    TargetRotation.Roll = 0.0f;

    // ✅ 부드러운 회전 적용 (즉각 회전 X, 자연스럽게 회전 O)
    FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
    SetActorRotation(NewRotation);

    // ✅ 이동 적용 (바라보는 방향으로 이동)
    AddMovementInput(MoveDirection, 1.0f);
>>>>>>> Stashed changes
}



<<<<<<< Updated upstream
=======



>>>>>>> Stashed changes
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