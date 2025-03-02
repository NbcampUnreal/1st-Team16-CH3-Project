
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