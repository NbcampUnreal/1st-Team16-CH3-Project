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