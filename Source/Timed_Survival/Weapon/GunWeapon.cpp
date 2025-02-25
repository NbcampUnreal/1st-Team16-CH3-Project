
#include "GunWeapon.h"

AGunWeapon::AGunWeapon()
{
	Damage = 10;
	ReloadDelay = 2.0f;
}

float AGunWeapon::GetReloadDelay() const
{
	return ReloadDelay;
}
