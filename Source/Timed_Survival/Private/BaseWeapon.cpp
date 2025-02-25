
#include "BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 10;
}

int32 ABaseWeapon::GetDamage() const
{
	return Damage;
}

