
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

public:	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void FireBullet();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual int32 GetDamage() const;

	void SetOwner(AActor* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeaponToCharacter(ACharacter* Character, FName SocketName);

};
