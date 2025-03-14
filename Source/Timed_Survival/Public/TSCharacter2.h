#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "GunWeapon.h"
#include "Sound/SoundCue.h"
#include "TSCharacter2.generated.h"


class USpringArmComponent;
class UCameraComponent;
class ABaseWeapon;
class UAniMontage;
class FTimerhandle;
class USoundCue;
struct FInputActionValue;

UCLASS()
class TIMED_SURVIVAL_API ATSCharacter2 : public ACharacter
{
	GENERATED_BODY()

public:

	ATSCharacter2();

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

	// 기본 카메라 위치 저장
	FVector DefaultCameraOffset;

	// 조준 시 카메라 위치
	FVector AimCameraOffset;

	// 위를 쳐다볼 수 있는 최대각도 (AO_Up 최대각도)
	float MaxLookUpAngle = 30.0f;

	// 아래를 쳐다볼 수 있는 최대각도 (AO_Down 최대각도)
	float MaxLookDownAngle = -30.0f;

public:
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

	// TakeDamage Animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* TakeDamageAnimation;


	// Aim Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRotator AimRotation;

	// Reload Charcter Move Stop and resetreload
	FTimerHandle ReloadTimerHandle;
	// Reset Fire
	FTimerHandle FireTimerHandle;
	// 샷건 나가는 시간 멈춰주는 타이머핸들
	FTimerHandle ShotgunCooldownTimerHandle;
	FTimerHandle FootsetpTimerHandle;

	// 걷기 재생사운드용 타이머핸들
	FTimerHandle FootstepTimerHandle;

	// 캐릭터가 맞으면 0.7초간 멈추게하기위한 타이머핸들
	FTimerHandle DamageMoveTimerHandle;


public:

	// 발사하는지 애니메이션 확인용 -> 변경 가능하게
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bFire = false;
	// 장전하는 애니메이션 계속 안나오게하는 bool변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsReloading = false;
	// 달리는지 확인하기위한 bool
	bool bCanPlayFootstep = true;

	// 최대 총알
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotGun")
	int32 MaxShotGunBullet = 2;
	// 현재 총알
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShotGun")
	int32 CurrentShotGunBullet = MaxShotGunBullet;

	// 조준 시 적용할 FOV
	float AimFOV = 80.0f;

	//Mvoe Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float NormalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementSpeed")
	float SprintSpeed;

	// 사격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireSound;
	// 걷는 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* WalkSound;
	// 뛰는 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SprintSound;
	// 체력이 감소될때 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* GruntSound;

	// ==============================================================================================================
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


public:

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

	//테스트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* WeaponChildActor;

	// 다시 총을 안쏘는 상태로 돌아가는 함수
	void ResetFireState();
	// 재장전 애니메이션이 계속 안나오도록 false하는 함수
	void ResetReloadState();

	// 캐릭터 애니메이션 블루프린트사용하기위해 현재 총알수를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	int32 GetCurrentShotGunBullet() const;

	// 이동 사운드 추가용 함수
	void PlayFootstepSound();

	void ResetFootStep();

	// 데미지 받으면 맞는 애니메이션 실행
	void TakeDamageAnim();

	void EnableMovementAfterDamage();
};