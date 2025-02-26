// Fill out your copyright notice in the Description page of Project Settings.


#include "TSPistolBulletItem.h"
#include "GameFramework/Actor.h"
#include "Pistol.h"

ATSPistolBulletItem::ATSPistolBulletItem()
{
	BulletAmount = 5;
	ItemType = FName("PistolBullet");
}

void ATSPistolBulletItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

    if (!Activator)
    {
        return;
    }

    APistol* EquippedPistol = Cast<APistol>(Activator);
    if (EquippedPistol)
    {
        int32 NewBulletCount = FMath::Clamp(EquippedPistol->GetBulletCount() + BulletAmount, 0, EquippedPistol->GetMaxBulletCount());
        EquippedPistol->SetBulletCount(NewBulletCount);

        Destroy();
    }
}