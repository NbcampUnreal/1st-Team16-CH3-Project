
#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"

#include "GunWeapon.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AGunWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxBulletCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> BulletClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReload;

	FName WeaponType;  

	FTimerHandle ReloadTimerHandle;

public:
	AGunWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetReloadDelay() const;

	virtual void FireBullet() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();
	
	UFUNCTION()
	virtual void FinishReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")

	int32 GetBulletCount() const { return BulletCount; }
	int32 GetMaxBulletCount() const { return MaxBulletCount; }
	void SetBulletCount(int32 NewBulletCount) { BulletCount = NewBulletCount; }

	FName GetWeaponType() const { return weaponType; }
	void AddBullet(int32 Amount);
};
