
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