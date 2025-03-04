
#include "Pistol.h"

APistol::APistol()
{
    Damage = 10;
    ReloadDelay = 1.5f;
}

void APistol::FireBullet()
{
    Super::FireBullet();

}

void APistol::Reload()
{
    Super::Reload();
}