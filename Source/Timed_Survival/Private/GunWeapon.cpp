
#include "GunWeapon.h"
#include "TimerManager.h"

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
	if (bIsReload)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire reload"));
		return;
	}

	if (BulletCount <= 0)
	{
		return;
	}

	Super::FireBullet();

	//총구 소켓 확인용
	if (!WeaponMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponMesh None"));
		return;
	}

	if (!WeaponMesh->DoesSocketExist("FirePoint"))
	{
		UE_LOG(LogTemp, Warning, TEXT("FirePoint socket No!!"));
		return;
	}

	FVector FirePointLocation = WeaponMesh->GetSocketLocation("FirePoint");
	FRotator FirePointRotation = WeaponMesh->GetSocketRotation("FirePoint");

	if (BulletClass)
	{
		AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, FirePointLocation, FirePointRotation);
		if (SpawnedBullet)
		{
			SpawnedBullet->SetOwner(this);
			BulletCount--;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed spawn bullet!"));
		}
	}
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