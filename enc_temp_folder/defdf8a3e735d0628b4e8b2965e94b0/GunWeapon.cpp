
#include "GunWeapon.h"
#include "TSAmmo.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "TSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"



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

	UWorld* World = GetWorld();
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
		AActor* OwnerActor = GetOwner();
		if (OwnerActor)
		{
			CameraLocation = OwnerActor->GetActorLocation();
			CameraRotation = OwnerActor->GetActorRotation();
		}
		else
		{
			return;
		}

		ShotDirection = CameraRotation.Vector();
	}

	FVector SpawnLocation = GetActorLocation() + (ShotDirection * 50.f);


	DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + (ShotDirection * 1000.0f),
		FColor::Red, false, 1.0f, 0, 3.0f);

	//  총알 스폰
	AActor* Bullet = World->SpawnActor<AActor>(BulletClass, SpawnLocation, CameraRotation);
	if (!Bullet)
	{
		UE_LOG(LogTemp, Error, TEXT(" FireBullet(): 총알 스폰 실패!"));
		return;
	}

	AActor* GunOwner = GetOwner();
	APawn* GunOwnerPawn = Cast<APawn>(GunOwner);

	if (GunOwnerPawn)
	{
		Bullet->SetOwner(GunOwnerPawn);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FireBullet(): GunWeapon의 Owner가 Pawn이 아닙니다!"));
	}

	UProjectileMovementComponent* ProjectileComp = Bullet->FindComponentByClass<UProjectileMovementComponent>();
	if (ProjectileComp)
	{
		ProjectileComp->Velocity = ShotDirection * ProjectileComp->InitialSpeed;
	}

	UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알 스폰 성공! %s"), *Bullet->GetName());


	//문제
	ATSAmmo* Ammo = Cast<ATSAmmo>(Bullet);
	float RandomDamage = FMath::RandRange(MinDamage, MaxDamage);
	if (Ammo)
	{
		Ammo->SetDamage(RandomDamage);
		UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알의 데미지 설정 완료 - %f"), RandomDamage);
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