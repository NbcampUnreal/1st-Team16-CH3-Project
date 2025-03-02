
#include "BaseWeapon.h"
#include "GameFramework/Character.h"
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

void ABaseWeapon::AttachWeaponToCharacter(ACharacter* Character, FName SocketName)
{
	if (!Character) return;

	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (!CharacterMesh) return;

	AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

