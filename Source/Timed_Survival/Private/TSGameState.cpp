#include "TSGameState.h"
#include "TSGameInstance.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ATSGameState::ATSGameState()
{
	HealingCount = 0;
}

void ATSGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartLevel();

	GetWorldTimerManager().SetTimer( //HUD Timer
		HUDUpdateTimerHandle,
		this,
		&ATSGameState::UpdateHUD,
		0.1f, true);
}

//about Game flow
void ATSGameState::StartLevel()
{
	UpdateHUD();
}
void ATSGameState::OnGameOver()
{
	UpdateHUD();
}
void ATSGameState::NextLevel()
{
}
void ATSGameState::EndLevel()
{
}
void ATSGameState::OnHPZero()
{
}
void ATSGameState:: BattleSystem()
{

}


// about UI Function
void ATSGameState::UpdateHUD()
{

}







// 시간(체력) 증가함수
void ATSGameState::IncreaseTime(float Value)
{
	// 시간 증가
}



int32 ATSGameState::GetHealingCount() const
{
	return HealingCount;
}


void ATSGameState::IncreaseHealingCount(int32 Amount)
{
	
	UTSGameInstance* GameInstance = Cast<UTSGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->AddToHealingCount(Amount);
	}
	
	// 디버깅 로그
	UE_LOG(LogTemp, Warning, TEXT("HealingCount: %d"), HealingCount);
}