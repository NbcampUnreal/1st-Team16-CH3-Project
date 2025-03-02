
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
	MinDamage = 5.0f;
	MaxDamage = 15.0f;
	ReloadDelay = 2.0f;
	bIsReload = false;
}

void AGunWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGunWeapon::Fire()
{
	if (BulletCount <= 0)
	{
		Reload();
		return;
	}

	FireBullet();
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
	UE_LOG(LogTemp, Warning, TEXT(" FireBullet() 실행됨!"));

	//  탄약 부족 시 발사 불가
	if (BulletCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT(" FireBullet(): 탄약 없음! 발사 불가!"));
		return;
	}

	//  총알 클래스 설정 확인
	if (!BulletClass)
	{
		UE_LOG(LogTemp, Error, TEXT(" FireBullet(): BulletClass가 설정되지 않음! 발사 불가!"));
		return;
	}

	//  탄환 개수 감소
	BulletCount--;
	UE_LOG(LogTemp, Warning, TEXT(" FireBullet(): 탄약 감소 - 현재 남은 탄약: %d"), BulletCount);

	//  총알 스폰 위치 및 회전값 설정
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = GetActorRotation();

	//  스폰 시도 로그
	UE_LOG(LogTemp, Warning, TEXT(" FireBullet(): 총알 스폰 시도! 위치: %s, 회전: %s"),
		*SpawnLocation.ToString(), *SpawnRotation.ToString());

	//  총알 스폰
	AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLocation, SpawnRotation);
	if (!Bullet)
	{
		UE_LOG(LogTemp, Error, TEXT(" FireBullet(): 총알 스폰 실패! BulletClass: %s, 위치: %s, 회전: %s"),
			*BulletClass->GetName(), *SpawnLocation.ToString(), *SpawnRotation.ToString());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알 스폰 성공! %s"), *Bullet->GetName());
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
	int32 NeededBullets = MaxBulletCount - BulletCount;

	if (ReserveBullet >= NeededBullets)
	{
		BulletCount = MaxBulletCount;
		ReserveBullet -= NeededBullets;
	}
	else
	{
		BulletCount += ReserveBullet;
		ReserveBullet = 0;
	}

	bIsReload = false;
}