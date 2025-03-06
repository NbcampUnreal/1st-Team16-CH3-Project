
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


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ATSAmmo> BulletClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReload;

	// 발사 상태를 확인하기위한 boolean변수
	bool bIsFiring = false;

	FTimerHandle ReloadTimerHandle;

	// M16용 발사속도 타이머핸들
	FTimerHandle FireTimerHandle;

public:
	AGunWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MinDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxBulletCount; // 총알 데이터가 필요해서 퍼블릭으로 뺐습니다 -전보경

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

	// M16용 총 발사 스피드 조절용 함수
	void StartFire();
	// M16용 사격 중지 함수
	void StopFire();

	int32 GetBulletInPlayer() const { return ReserveBullet; }
	int32 GetBulletCount() const { return BulletCount; }
	int32 GetMaxBulletCount() const { return MaxBulletCount; }
	void SetBulletCount(int32 NewBulletCount) { BulletCount = NewBulletCount; }

	FName GetWeaponType() const { return WeaponType; }
	void AddBullet(int32 Amount);

protected:
	virtual void BeginPlay() override;
};
