// Fill out your copyright notice in the Description page of Project Settings.


#include "TSMaskItem.h"
#include "TSGameState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ATSMaskItem::ATSMaskItem()
{
	StopTimeDuration = 30.0f;
	ItemType = FName(TEXT("Mask"));
}

void ATSMaskItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (GameState)
        {
            GameState->SetStopTimeReductionEnabled(false); // 체력 감소 멈춤
            GameState->SetMaskEffect(true, StopTimeDuration); // HUD에 마스크 효과 표시
        }

        DestroyItem(); // 아이템 제거
    }
}
