#include "TSGameState.h"
#include "TSGameInstance.h"
#include "TSCharacter.h"
#include "TSPlayerController.h"
#include "TSBaseBulletItem.h"
#include "TSARBulletItem.h"
#include "TSPistolBulletItem.h"
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

}

void ATSGameState::OnGameOver()
{
	UpdateHUD();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			TSPlayerController->ShowMainMenu();

			// 죽는 애니메이션이 끝나는 3초 뒤에 SetPause(true)가 실행하도록함.
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateWeakLambda(TSPlayerController,[TSPlayerController](){TSPlayerController->SetPause(true);}),
				1.7f,
				false
			);
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

	// 캐릭터의 Deaht()함수 호출로 죽는 애니메이션 재생용
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ATSCharacter* PlayerCharacter = Cast<ATSCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->Death();
		}
	}
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
				// 1) About Health HUD========================================================================================
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

				// 2) About Bullet HUD========================================================================================
				GetBulletData();

				// 2)-1 AR Bullet Count
				if (UTextBlock* CountARBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ARBullet"))))
				{
					//CountARBullet->SetText(FText::FromString(FString::Printf(TEXT("X %d"), Bundle)));
				}
				// 2)-2 AR Bullet Reload
				if (UTextBlock* CountBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ARBulletReload"))))
				{

				}
				// 2)-3 Pistol Bullet Count
				if (UTextBlock* CountBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PistolBullet"))))
				{

				}
				// 2)-4 Pistol Bullet Reload
				if (UTextBlock* CountBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PistolBulletReload"))))
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

//----------------------- 방독면 ------------------------

// 방독면 활성화 함수
void ATSGameState::SetStopTimeReductionEnabled(bool bEnable)
{
	bIsStopTimeReductionEnabled = bEnable;
}

// 방독면 효과 함수
void ATSGameState::SetMaskEffect(bool bEnable, float Duration)
{
	bIsMaskActive = bEnable;
	MaskTimeRemaining = Duration;

	if (bEnable)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MaskEffectTimerHandle,
			this,
			&ATSGameState::UpdateMaskTimer,
			1.0f,
			true
		);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(MaskEffectTimerHandle);
		MaskTimeRemaining = 0.0f;
	}

	UpdateHUD();
}

// 방독면 남은 시간 반환 함수
void ATSGameState::UpdateMaskTimer()
{
	if (MaskTimeRemaining > 0)
	{
		MaskTimeRemaining -= 1.0f;
		UpdateHUD();
	}
	else
	{
		SetMaskEffect(false, 0.0f);
		SetStopTimeReductionEnabled(true);
	}
}

// 방독면 활성화 여부 반환 함수
bool ATSGameState::IsMaskActive() const
{
	return bIsMaskActive;
}
// -----------------------------------------------------

// 시간(체력) 증가함수
void ATSGameState::IncreaseTime(float Value)
{
	ItemHealth += Value;
	UpdateHealth();
}

// 시간(체력) 감소함수
void ATSGameState::ReduceTime(float Value, bool bIgnoreMask)
{
	// bIgnoreMask가 true일 떄 지뢰 같은 강제 피해는 적용되도록 예외 처리
	if (bIsStopTimeReductionEnabled || bIgnoreMask)
	{
		ItemHealth -= Value;
		UpdateHealth();
	}
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

void ATSGameState::UpdateHealth()
{
	CurrentHealth = BaseHealth * 60.0f + ItemHealth;
}

void ATSGameState::SubtractHealthOnSecond()
{
	if (bIsStopTimeReductionEnabled && CurrentHealth > 0.0f)
	{
		CurrentHealth -= 0.1f;
	}

	if (CurrentHealth <= 0.0f)
	{
		// 0초 밑으로 안내려가게 Clear
		GetWorldTimerManager().ClearTimer(SubtractHealthTimerHandle); 

		OnHPZero();
	}
}

// GunWeapon Bullet Data Function

void ATSGameState::GetBulletData()
{
	AGunWeapon* BulletData = GetWorld()->SpawnActor<AGunWeapon>(AGunWeapon::StaticClass());
	BulletData->GetBulletInPlayer();
	BulletData->GetBulletCount();
	BulletData->GetMaxBulletCount();
	BulletData->GetWeaponType();
}

void ATSGameState::UpdateBulletData()
{
	GetBulletData();
}

// void ATSGameState::FindARBullet()
// {
// 	TArray<AActor*> FoundBullets;
// 	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),TSubclassOf<AActor>ATSBaseBulletItem,)
// }
// void ATSGameState::FindPistolBullet()
// {
// }
