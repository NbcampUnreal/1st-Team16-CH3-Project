// Fill out your copyright notice in the Description page of Project Settings.


#include "TSPistolBulletItem.h"

ATSPistolBulletItem::ATSPistolBulletItem()
{
	BulletAmount = 5;
	ItemType = FName(TEXT("Pistol"));
	//WeaponType = FName(TEXT("Pistol"));
}

void ATSPistolBulletItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}