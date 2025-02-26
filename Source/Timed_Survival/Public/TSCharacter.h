#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float NormalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementSpeed")
	float SprintSpeed;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

};