#include "GunWeapon.h"
#include "TSGameState.h"
#include "TSAmmo.h"
#include "TSCharacter.h"  // 🔹 여기에서만 포함하면 순환 종속 문제 해결
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AGunWeapon::AGunWeapon()
{
	ReserveBullet = 30; // 예비 탄약
	BulletCount = 30;
	MaxBulletCount = 30;
	Damage = 10;
	MinDamage = 10.0f;
	MaxDamage = 20.0f;
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
    UE_LOG(LogTemp, Warning, TEXT("FireBullet() 실행됨!"));

    // 탄약 부족 시 발사 불가
    if (BulletCount <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet(): 탄약 없음! 발사 불가!"));
        return;
    }

    // 총알 클래스 설정 확인
    if (!BulletClass)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet(): BulletClass가 설정되지 않음! 발사 불가!"));
        return;
    }

    // 탄환 개수 감소
    BulletCount--;
    UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 탄약 감소 - 현재 남은 탄약: %d"), BulletCount);

    ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GameState)
    {
        GameState->SetM16BulletCount(BulletCount); // 탄약증가
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet(): GetWorld()를 찾을 수 없습니다!"));
        return;
    }

    // 총알 스폰 위치 및 회전값 설정
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

    // 총알 스폰
    AActor* Bullet = World->SpawnActor<AActor>(BulletClass, SpawnLocation, CameraRotation);
    if (!Bullet)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet(): 총알 스폰 실패!"));
        return;
    }

    ATSAmmo* Ammo = Cast<ATSAmmo>(Bullet);
    if (Ammo)
    {
        UProjectileMovementComponent* ProjectileComp = Ammo->GetProjectileMovementComponent();
        if (ProjectileComp)
        {
            FVector ForwardVector = CameraRotation.Vector();
            ProjectileComp->SetVelocityInLocalSpace(ForwardVector * ProjectileComp->InitialSpeed);
            ProjectileComp->Activate();
            UE_LOG(LogTemp, Warning, TEXT("총알이 발사됨! 방향: %s, 속도: %f"), *ForwardVector.ToString(), ProjectileComp->InitialSpeed);
        }

        AActor* OwnerActor = GetOwner(); //  소유자 가져오기
        if (OwnerActor)
        {
            Ammo->CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
            UE_LOG(LogTemp, Warning, TEXT("총알이 %s(소유자)와 충돌을 무시합니다."), *GetOwner()->GetName())
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GetOwner()가 nullptr입니다! 총알과의 충돌 무시 불가"));
        }

        int32 RandomDamage = FMath::RandRange(MinDamage, MaxDamage);
        Ammo->SetDamage(RandomDamage);
        UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알의 데미지 설정 완료 - %d"), RandomDamage);
    }

    UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 총알 스폰 성공! %s"), *Bullet->GetName());

    // 무기를 소유한 캐릭터 가져오고
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        ATSCharacter* PlayerCharacter = Cast<ATSCharacter>(OwnerCharacter);
        if (PlayerCharacter)
        {
            // SetCurrentBullet함수를 들고와서 캐릭터에CurrentBullet 업데이트
            PlayerCharacter->SetCurrentBullet(BulletCount);
            UE_LOG(LogTemp, Warning, TEXT("FireBullet(): 플레이어 탄약 개수 업데이트 - %d"), PlayerCharacter->GetCurrentBullet());
        }
    }

    Bullet->SetOwner(this);

    // 데미지 설정 (옵션)
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

    ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GameState)
    {
        GameState->SetM16BulletCount(BulletCount);
    }

	UE_LOG(LogTemp, Warning, TEXT("FinishReload(): 리로드 후 탄약: %d, 예비 탄약: %d"), BulletCount, ReserveBullet);
}

void AGunWeapon::StartFire()
{
    if (bIsFiring || BulletCount <= 0 || bIsReload) return;

    bIsFiring = true; 
    FireBullet(); // 즉시 1발 발사

    GetWorld()->GetTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AGunWeapon::FireBullet,
        0.14f, // 0.14초 간격으로 반복해서 총쏘는 애니메이션이랑 비슷하게 발사되게 설정
        true // 반복 실행설정
    );
}

void AGunWeapon::StopFire()
{
    // 발사 중이 아니면 실행 X.
    if (!bIsFiring) return; 

    bIsFiring = false; 
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}