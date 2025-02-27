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
struct FInputActionValue;

UCLASS()
class TIMED_SURVIVAL_API ATSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATSCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<AGunWeapon*> Weapons;

	// Death Aniamtion
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathAnimation;

	// Aim Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRotator AimRotation;

public:

	//Mvoe Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float NormalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementSpeed")
	float SprintSpeed;

	// Time Health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHP;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

public:
	
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

	AGunWeapon* FindWeaponByType(FName WeaponType);

	// Death 
	UFUNCTION()
	void Death();

	// Mouse Move Aim Function
	void UpdateAimOffset();

};