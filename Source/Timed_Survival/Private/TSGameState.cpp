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
	HealthBarMax = 180.0;
	BaseHealth = 3.0f;
	ItemHealth = 0.0f;
	CurrentHealth = BaseHealth * 60.0f;
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
	GetWorldTimerManager().SetTimer( // Subtract Time
		SubtractHealthTimerHandle,
		this,
		&ATSGameState::SubtractHealthOnSecond,
		0.1f,
		true);

	//UpdateHUD();
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
				float TotalHealthSet = CurrentHealth / HealthBarMax; // HealthBar 비율 세팅
				int32 Bundle = (int32)TotalHealthSet; // HealthBar 몫
				float HealthPercent = TotalHealthSet - Bundle; // HealthBar 반영 비율

				//1)-1 Health Text
				if (UTextBlock* HealthText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HealthText"))))
				{
					HealthText->SetText(FText::FromString(FString::Printf(TEXT("HealthTest : %.1f"),CurrentHealth)));
				}

				// 1)-2 Health Bar percent
				if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
				{
					HealthBar->SetPercent(HealthPercent);
				}

				// 1)-3 Health Bundle Text
				if (UTextBlock* HealthBundle = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HealthBundle"))))
				{
					HealthBundle->SetText(FText::FromString(FString::Printf(TEXT("X %d"), Bundle)));
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
	ItemHealth += Value;
	UpdateHealth();
}

// 시간(체력) 감소함수
void ATSGameState::ReduceTime(float Value)
{
  ItemHealth -= Value;
	UpdateHealth();
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

void ATSGameState::SubtractHealthOnSecond()
{
	CurrentHealth -= 0.1f;
}

void ATSGameState::UpdateHealth()
{
	CurrentHealth = BaseHealth * 60.0f + ItemHealth;
}