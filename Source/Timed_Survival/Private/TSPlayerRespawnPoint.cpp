// Fill out your copyright notice in the Description page of Project Settings.

#include "TSPlayerRespawnPoint.h"
#include "TSGameState.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"


ATSPlayerRespawnPoint::ATSPlayerRespawnPoint()
{

	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATSPlayerRespawnPoint::OnOverlap);

	bIsActivated = false; // 초기에는 비활성화 상태
}


void ATSPlayerRespawnPoint::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//if (bIsActivated)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Respawn Point already activated. Ignoring overlap."));
	//	return; // 이미 활성화된 리스폰 지점이면 무시
	//}

	//if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	//{
	//	ATSGameState* GameState = GetWorld()->GetGameState<ATSGameState>();
	//	if (GameState)
	//	{
	//		ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	//		if (PlayerCharacter)
	//		{
	//			FVector RespawnLocation = PlayerCharacter->GetActorLocation(); // 플레이어 위치 가져오기
	//			float PlayerHealth = GameState->CurrentHealth; // GameState에서 현재 체력 가져오기

	//			// 새로운 리스폰 지점이 활성화되면 기존 값 덮어씌움
	//			GameState->SetRespawnPoint(PlayerCharacter->GetActorLocation(), PlayerHealth);
	//			bIsActivated = true; // 한 번 활성화되면 다시 발동하지 않도록 설정

	//			UE_LOG(LogTemp, Warning, TEXT("Respawn Point Activated at Location: %s"), *RespawnLocation.ToString());
	//			UE_LOG(LogTemp, Warning, TEXT("Player Health saved: %f"), PlayerHealth);
	//		}
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT("OnOverlap called on TSPlayerRespawnPoint"));

	if (bIsActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Respawn Point already activated. Ignoring overlap."));
		return;
	}

	if (!OtherActor)
	{
		UE_LOG(LogTemp, Error, TEXT("RespawnPoint: Overlapped actor is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());

	if (!OtherActor->ActorHasTag(TEXT("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped actor does not have 'Player' tag. Ignoring."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player overlapped with Respawn Point! Checking GameState..."));

	ATSGameState* GameState = GetWorld()->GetGameState<ATSGameState>();
	if (!GameState)
	{
		UE_LOG(LogTemp, Error, TEXT("RespawnPoint: Could not get TSGameState!"));
		return;
	}

	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("RespawnPoint: Overlapped actor is not a character!"));
		return;
	}

	FVector RespawnLocation = PlayerCharacter->GetActorLocation();
	float PlayerHealth = GameState->CurrentHealth;

	UE_LOG(LogTemp, Warning, TEXT("Setting Respawn Point at: %s"), *RespawnLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Saving Player Health: %f"), PlayerHealth);

	GameState->SetRespawnPoint(RespawnLocation, PlayerHealth);
	bIsActivated = true; // 한 번 활성화되면 다시 발동하지 않도록 설정

	UE_LOG(LogTemp, Warning, TEXT("Respawn Point successfully activated!"));
}
