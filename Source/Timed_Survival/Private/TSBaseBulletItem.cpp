// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBaseBulletItem.h"
#include "BaseWeapon.h"
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
        //    ABaseWeapon* BaseWeapon = Cast<ABaseWeapon>(WeaponActor);
        //    if (BaseWeapon)
        //    {
        //        // FName을 비교하여 해당 무기의 탄약을 추가
        //        if (ItemType == FName("PistolBullet"))
        //        {
        //            BaseWeapon->PistolBulletAmount += BulletAmount;
        //        }
        //        else if (ItemType == FName("ARBullet"))
        //        {
        //            BaseWeapon->ARBulletAmount += BulletAmount;
        //        }
        //    }
        //}

        DestroyItem();
    }
}