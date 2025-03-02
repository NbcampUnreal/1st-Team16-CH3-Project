
#include "GunWeapon.h"
#include "TSAmmo.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGunWeapon::AGunWeapon()
{
	ReserveBullet = 10; // 예비 탄약
	BulletCount = 10;
	MaxBulletCount = 10;
	Damage = 10;
	ReloadDelay = 2.0f;
	bIsReload = false;
}

void AGunWeapon::AddBullet(int32 Amount)
{
	ReserveBullet += Amount; // 예비 탄약에 추가
	//디버그용
	UE_LOG(LogTemp, Warning, TEXT("%s 탄약 추가됨! 현재 탄약: %d"), *WeaponType.ToString(), ReserveBullet);
}

float AGunWeapon::GetReloadDelay() const
{
	return ReloadDelay;
}

void AGunWeapon::FireBullet()
{
	//reload
	if (bIsReload || BulletCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire reload"));
		return;
	}

	BulletCount--;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = GetActorRotation();

	if (BulletClass)
	{
		ATSAmmo* Bullet = GetWorld()->SpawnActor<ATSAmmo>(BulletClass, SpawnLocation, SpawnRotation);
		if (Bullet)
		{
			UProjectileMovementComponent* BulletMovement = Bullet->GetProjectileMovementComponent();
			if (BulletMovement)
			{
				BulletMovement->InitialSpeed = 5000.f;
				BulletMovement->MaxSpeed = 5000.f;
			}
		}
	}

	Super::FireBullet();
}


void AGunWeapon::Reload()
{
	if (bIsReload || BulletCount == MaxBulletCount)
	{
		return;
	}

	bIsReload = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGunWeapon::FinishReload, ReloadDelay, false);
}

void AGunWeapon::FinishReload()
{
	BulletCount = MaxBulletCount;
	bIsReload = false;
}