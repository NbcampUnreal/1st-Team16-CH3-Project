// Fill out your copyright notice in the Description page of Project Settings.


#include "TSARBulletItem.h"
#include "GameFramework/Actor.h"
#include "AR.h"

ATSARBulletItem::ATSARBulletItem()
{
	BulletAmount = 10;
	ItemType = FName("ARBullet");
}

void ATSARBulletItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

    if (!Activator)
    {
        return;
    }

    AAR* EquippedAR = Cast<AAR>(Activator);
    if (EquippedAR)
    {
        int32 NewBulletCount = FMath::Clamp(EquippedAR->GetBulletCount() + BulletAmount, 0, EquippedAR->GetMaxBulletCount());
        EquippedAR->SetBulletCount(NewBulletCount);

        Destroy();
    }
}