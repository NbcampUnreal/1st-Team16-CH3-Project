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
class AGunWeapon;
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

	// 기본 카메라 위치 저장
	FVector DefaultCameraOffset;

	// 조준 시 카메라 위치
	FVector AimCameraOffset;

	// 위를 쳐다볼 수 있는 최대각도 (AO_Up 최대각도)
	float MaxLookUpAngle = 30.0f;

	// 아래를 쳐다볼 수 있는 최대각도 (AO_Down 최대각도)
	float MaxLookDownAngle = -30.0f;

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


	// Aim Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRotator AimRotation;

	// Reload Charcter Move Stop and resetreload
	FTimerHandle ReloadTimerHandle;
	// Reset Fire
	FTimerHandle FireTimerHandle;

public:

	// 발사하는지 애니메이션 확인용 -- 변경 가능하게
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bFire = false;
	
	// 조준 시 적용할 FOV
	float AimFOV = 80.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CurrentBullet")
	// GunWeapon에 총알 갯수를 반환해주는 변수 -- ABP_에서 사용해야해서 UPROPERTY로 생성
	int32 CurrentBullet;

	//Mvoe Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float NormalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementSpeed")
	float SprintSpeed;


	// about Health
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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
	void StartFire(const FInputActionValue& value);
	UFUNCTION()
	void StopFire(const FInputActionValue& value);
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

	// 총 발사상태를 안쏘는 상태로 초기화해주는 함수
	void ResetFireState();

	// CurrentBullet 값을 설정하는 함수 추가
	void SetCurrentBullet(int32 NewBulletCount)
	{
		CurrentBullet = NewBulletCount;
	}

	// 현재 탄약 개수를 가져오는 Getter 추가
	int32 GetCurrentBullet() const
	{
		return CurrentBullet;
	}
};