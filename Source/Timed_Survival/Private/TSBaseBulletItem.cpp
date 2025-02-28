// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBaseBulletItem.h"
#include "TSCharacter.h"
#include "GunWeapon.h"
#include "TSGameState.h"
#include "Kismet/GameplayStatics.h"

ATSBaseBulletItem::ATSBaseBulletItem()
{
	BulletAmount = 10; // 총알 수량
    WeaponType = FName("DefaultBullet");
}


void ATSBaseBulletItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (!Activator)
    {
        return;
    }

    // Activator를 TSCharacter로 캐스팅
    ATSCharacter* Player = Cast<ATSCharacter>(Activator);
    if (Player)
    {
        // 플레이어가 소지한 무기 중 해당 탄약이 들어갈 무기 찾기
        AGunWeapon* Weapon = Player->FindWeaponByType(WeaponType);
        if (Weapon)
        {
            Weapon->AddBullet(BulletAmount);
            
        }
    }
     // Call GameState
    ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GameState)
    {
        GameState->GetBulletData();
    }


    DestroyItem();
}