#include "TSGameState.h"
#include "TSGameInstance.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
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

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			TSPlayerController->SetPause(true);
			TSPlayerController->ShowMainMenu();
		}
	}
}
void ATSGameState::NextLevel()
{

}
void ATSGameState::EndLevel()
{

}
void ATSGameState::OnHPZero()
{
	OnGameOver();
}
void ATSGameState:: BattleSystem()
{

}


// about UI Function
void ATSGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = TSPlayerController->GetHUDWidget())
			{
				//HP바 수치 적용 체크용 if문입니다. 나중에 주석처리 or 삭제
				if (UTextBlock* HPTextCheck = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HPCheck"))))
				{
					float RemainingHP = GetWorldTimerManager().GetTimerRemaining(HPTimerHandle);
					HPTextCheck->SetText(FText::FromString(FString::Printf(TEXT("HP %1.f"), RemainingHP)));
				}
				//HP바 수치 적용 체크용 if문입니다. 나중에 주석처리 or 삭제


				// 1) HP Bar
				if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HPBar"))))
				{
					float RemainingHP = GetWorldTimerManager().GetTimerRemaining(HPTimerHandle);
					float HpPercent = RemainingHP / MaxHP; //체력 증가, 디버프, 감소 수치 추가해야됨
					HPBar->SetPercent(HpPercent);
				}

				// 2) 총알 수
				if (UTextBlock* CountBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CountBullet"))))
				{
					
				}
				
				// 3) 회복 시 UI효과
				// 4) 디버프 시 UI효과(지속)
				// 5) 피격 시 UI효과(지뢰, AI)(일시적)
				// 6) 헤드샷 UI효과
			}
		}
	}
}