#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "GunWeapon.h"
#include "TSCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class ABaseWeapon;
class UAniMontage;
class FTimerhandle;
struct FInputActionValue;

UCLASS()
class TIMED_SURVIVAL_API ATSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ATSCharacter();

private:
	// 총을 쏘고있는지 판단
	bool IsFiring = false;

	// 조준을 하고있는지 판단
	bool bIsAiming = false;

	// 이동키를 입력하고 있는지 확인
	bool IsMovingForward = false;

	// 마지막 입력된 이동 값 저장
	FVector2D LastMoveInput; 

	// 마지막 입력된 이동 방향 저정
	FVector LastMoveDirection;

	// 기본 FOV 저장
	float DefaultFOV;   

	// 조준 시 적용할 FOV
	float AimFOV = 60.0f;   

	// 기본 카메라 위치 저장
	FVector DefaultCameraOffset;

	// 조준 시 카메라 위치
	FVector AimCameraOffset;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	
	// Death Aniamtion
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathAnimation;

	// Reload Animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ReloadAnimation;
	// Fire Animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* FireAnimation;



	// Aim Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRotator AimRotation;

	// Reload Charcter Move Stop and resetreload
	FTimerHandle ReloadTimerHandle;
	// Reset Fire
	FTimerHandle FireTimerHandle;

public:
	
	//Mvoe Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float NormalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementSpeed")
	float SprintSpeed;


	// about Health
	void TakeDamage();

	// 무기 리스트 담을 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<AGunWeapon*> Weapons;
	
	// 무기 타입으로 무기 찾는 함수(총알 추가용)
	AGunWeapon* FindWeaponByType(FName WeaponType);

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);
	UFUNCTION()
	void Fire(const FInputActionValue& value);
	UFUNCTION()
	void StartAiming(const FInputActionValue& value);
	UFUNCTION()
	void StopAiming(const FInputActionValue& value);



	// Death
	UFUNCTION()
	void Death();

	// 다시 이동할 수 있는 함수(Reload시에 이동이 불가하여)
	UFUNCTION()
	void EnableMovementAfterReload(); 

	// 캐릭터가 바라보는곳을 마우스로 설정하는 함수
	void FaceMouseDirection();

	// 총 쏜뒤에 캐릭터가 움직이게하는 함수
	void ResetMovementAfterFire();
};