#include "TSGameState.h"
#include "TSGameInstance.h"
#include "TSCharacter.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

ATSGameState::ATSGameState()
{
	HealingCount = 0;
	BaseHealth = 3.0;
	TotalHealth = BaseHealth * 60.0;
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

	GetWorldTimerManager().SetTimer( //HP Timer
		HealthTimerHandle,
		this,
		&ATSGameState::OnHPZero,
		TotalHealth,
		false
	);
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
				//1)-1 HP Text
				if (UTextBlock* HealthText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HealthText"))))
				{
					float RemainingHealth = GetWorldTimerManager().GetTimerRemaining(HealthTimerHandle);
					HealthText->SetText(FText::FromString(FString::Printf(TEXT("%1.f"), RemainingHealth)));
				}

				// 1)-2 HP Bar
				if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
				{
					float RemainingHealth = GetWorldTimerManager().GetTimerRemaining(HealthTimerHandle);
					float HealthPercent = RemainingHealth; //체력 증가, 디버프, 감소 수치 추가해야됨
					HealthBar->SetPercent(HealthPercent);
				}


				// 2) Bullet Count
				if (UTextBlock* CountBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CountBullet"))))
				{
					
				}
				
				// 3) Healing Effect
				// 4) Debuff
				// 5) 피격 시 UI효과(지뢰, AI)(일시적)
				// 6) 헤드샷 UI효과
			}
		}
	}

}
// 시간(체력) 증가함수
void ATSGameState::IncreaseTime(float Value)
{
	//TotalHealth += HealingAmount
}

int32 ATSGameState::GetHealingCount() const
{
	return HealingCount;
}

// 회복 수집 횟수 증가 함수
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