// Fill out your copyright notice in the Description page of Project Settings.


#include "TSMaskItem.h"
#include "TSGameState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ATSMaskItem::ATSMaskItem()
{
	StopTimeDuration = 30.0f;
	ItemType = FName("Mask");
}

void ATSMaskItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (GameState)
        {
            GameState->SetStopTimeReductionEnabled(false); // 시간 감소 멈춤

            // 30초 후 효과 해제
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                [GameState]()
                {
                    GameState->SetStopTimeReductionEnabled(true); // 다시 시간 감소 활성화
                },
                StopTimeDuration,
                false
            );
        }

        DestroyItem(); // 아이템 삭제
    }
}