
#include "GunWeapon.h"
#include "TSAmmo.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "TSCharacter.h"
#include "Camera/CameraComponent.h"


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

	UWorld* World = this->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("FireBullet(): GetWorld()를 찾을 수 없습니다!"));
		return;
	}

	//  총알 스폰 위치 및 회전값 설정
	FVector CameraLocation;
	FRotator CameraRotation;
	FVector ShotDirection = FVector::ZeroVector;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		ShotDirection = CameraRotation.Vector();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FireBullet(): 플레이어 컨트롤러를 찾을 수 없습니다! 기본값 사용."));

		AActor* OwnerActor = GetOwner();
		if (OwnerActor)
		{
			CameraLocation = OwnerActor->GetActorLocation();
			CameraRotation = OwnerActor->GetActorRotation();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FireBullet(): GetOwner()가 NULL입니다! 기본 위치 사용."));
			CameraLocation = FVector::ZeroVector;
			CameraRotation = FRotator::ZeroRotator;
		}

		ShotDirection = CameraRotation.Vector();
	}


	FVector SpawnLocation = GetActorLocation() + (ShotDirection * 50.f);

	//  스폰 시도 로그
	UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알 스폰 위치: %s, 방향: %s"),
		*SpawnLocation.ToString(), *ShotDirection.ToString());

	DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + (ShotDirection * 1000.0f),
		FColor::Red, false, 1.0f, 0, 3.0f);

	//  총알 스폰
	AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLocation, CameraRotation);
	if (!Bullet)
	{
		UE_LOG(LogTemp, Error, TEXT(" FireBullet(): 총알 스폰 실패! BulletClass: %s, 위치: %s, 회전: %s"),
			*BulletClass->GetName(), *SpawnLocation.ToString(), *CameraRotation.ToString());
		return;
	}

	UProjectileMovementComponent* ProjectileComp = Bullet->FindComponentByClass<UProjectileMovementComponent>();
	if (ProjectileComp)
	{
		ProjectileComp->Velocity = ShotDirection * ProjectileComp->InitialSpeed;
	}

	UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알 스폰 성공! %s"), *Bullet->GetName());
}


bool AGunWeapon::CanReload() const
{
	return !bIsReload && BulletCount < MaxBulletCount;
}

void AGunWeapon::Reload()
{
	if (!CanReload())
	{
		UE_LOG(LogTemp, Warning, TEXT("Reload(): 리로드 불가능!"));
		return;
	}

	if (bIsReload || BulletCount == MaxBulletCount)
	{
		return;
	}

	bIsReload = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGunWeapon::FinishReload, ReloadDelay, false);
}

void AGunWeapon::FinishReload()
{
	UE_LOG(LogTemp, Warning, TEXT("FinishReload(): 리로드 완료"));

	BulletCount = MaxBulletCount;

	bIsReload = false;

	UE_LOG(LogTemp, Warning, TEXT("FinishReload(): 리로드 후 탄약: %d, 예비 탄약: %d"), BulletCount, ReserveBullet);
}