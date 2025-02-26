
#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	Damage = 10;
}

void ABaseWeapon::Attack()
{
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

