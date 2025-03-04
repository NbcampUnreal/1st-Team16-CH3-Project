
#include "AR.h"

AAR::AAR()
{
	Damage = 30;
	ReloadDelay = 1.2f;
}

void AAR::FireBullet()
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

void AAR::Reload()
    {
        if (bIsReload || BulletCount == MaxBulletCount)
        {
            return;
        }

        bIsReload = true;

        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAR::FinishReload, ReloadDelay, false);
    }

    void AAR::FinishReload()
    {
        BulletCount = MaxBulletCount;
        bIsReload = false;

}