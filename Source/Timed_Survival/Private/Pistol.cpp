
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