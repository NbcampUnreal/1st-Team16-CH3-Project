
#include "Pistol.h"

APistol::APistol()
{
    Damage = 10;
    ReloadDelay = 1.5f;
}

void APistol::FireBullet()
{
    if (BulletCount <= 0)
    {
        return;
    }

    if (!BulletClass)
    {
        return;
    }

    FVector FirePointLocation = WeaponMesh->GetSocketLocation("FirePoint");
    FRotator FirePointRotation = WeaponMesh->GetSocketRotation("FirePoint");

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

void APistol::Reload()
{
    if (bIsReload || BulletCount == MaxBulletCount)
    {
        return;
    }

    bIsReload = true;

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &APistol::FinishReload, ReloadDelay, false);
}

void APistol::FinishReload()
{
    BulletCount = MaxBulletCount;
    bIsReload = false;
}