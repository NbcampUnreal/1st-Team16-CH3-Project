#include "TSGameState.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ATSGameState::ATSGameState()
{

}

void ATSGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartLevel();

	GetWorldTimerManager().SetTimer( //HUD 타이머
		HUDUpdateTimerHandle,
		this,
		&ATSGameState::UpdateHUD,
		0.1f, true);
}

//게임 흐름 관련 함수
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


// UI 관련 함수
void ATSGameState::UpdateHUD()
{

}