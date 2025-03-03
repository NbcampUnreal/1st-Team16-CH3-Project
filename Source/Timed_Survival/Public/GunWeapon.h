
#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "TSAmmo.h"
#include "GameFramework/Actor.h"
#include "GunWeapon.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AGunWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadDelay;

	// 예비 탄약
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 ReserveBullet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxBulletCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ATSAmmo> BulletClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReload;

	FTimerHandle ReloadTimerHandle;

public:
	AGunWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetReloadDelay() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanReload() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void FinishReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	virtual void FireBullet() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MaxDamage;

	int32 GetBulletInPlayer() const { return ReserveBullet; }
	int32 GetBulletCount() const { return BulletCount; }
	int32 GetMaxBulletCount() const { return MaxBulletCount; }
	void SetBulletCount(int32 NewBulletCount) { BulletCount = NewBulletCount; }

	FName GetWeaponType() const { return WeaponType; }
	void AddBullet(int32 Amount);

protected:
	virtual void BeginPlay() override;
};
