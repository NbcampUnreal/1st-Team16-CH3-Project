// Fill out your copyright notice in the Description page of Project Settings.


#include "TSGoalPoint.h"
#include "Components/BoxComponent.h"
#include "TSGameState.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATSGoalPoint::ATSGoalPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATSGoalPoint::OnOverlap);
}

void ATSGoalPoint::OnOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        // 플레이어가 목표 지점에 도착했을 때 게임 종료 처리
        ATSGameState* GameState = Cast<ATSGameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (GameState)
        {
            GameState->EndLevel();
        }
    }
}