// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBaseBulletItem.h"
#include "../Weapon/GunWeapon.h"
#include "GameFramework/Character.h"

ATSBaseBulletItem::ATSBaseBulletItem()
{
	BulletAmount = 10; // 총알 수량
	ItemType = "DefaultBullet";
}


void ATSBaseBulletItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(Activator);
        if (!PlayerCharacter)
        {
            return;
        }

        //// 임시 작성 코드
        // 
        //// 플레이어가 보유한 무기 탐색
        //TArray<AActor*> OwnedWeapons;
        //PlayerCharacter->GetAttachedActors(OwnedWeapons);

        //for (AActor* WeaponActor : OwnedWeapons)
        //{
        //    AGunWeapon* GunWeapon = Cast<AGunWeapon>(WeaponActor);
        //    if (GunWeapon)
        //    {
        //        // FName을 비교하여 해당 무기의 탄약을 추가
        //        if (ItemType == FName("PistolBullet"))
        //        {
        //            GunWeapon->PistolBulletAmount += BulletAmount;
        //        }
        //        else if (ItemType == FName("ARBullet"))
        //        {
        //            GunWeapon->ARBulletAmount += BulletAmount;
        //        }
        //    }
        //}

        DestroyItem();
    }
}