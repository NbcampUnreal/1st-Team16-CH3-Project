// Fill out your copyright notice in the Description page of Project Settings.


#include "TSARBulletItem.h"

ATSARBulletItem::ATSARBulletItem()
{
	BulletAmount = 10;
	ItemType = FName("ARBullet");
}

void ATSARBulletItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}