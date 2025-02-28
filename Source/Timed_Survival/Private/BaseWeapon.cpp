
#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Damage = 10;
}

void ABaseWeapon::FireBullet()
{
}

int32 ABaseWeapon::GetDamage() const
{
	return Damage;
}

void ABaseWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
}

