#include "TSGameState.h"
#include "TSGameInstance.h"
#include "TSBaseItem.h"
#include "TSCharacter.h"
#include "TSCharacter2.h"
#include "TSPlayerController.h"
#include "TSBaseBulletItem.h"
#include "TSARBulletItem.h"
#include "TSPistolBulletItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "TSUserWidgetManager.h"
#include "AI/TS_EnemySpawnVolume.h"
#include "AI/EnemyCharacter.h"
#include <Components/WidgetComponent.h>


ATSGameState::ATSGameState()
{	
	HealthBarMax = 180.0;
	BaseHealth = 3.0f; // unit of time : min
	ItemHealth = 0.0f;
	CurrentHealth = BaseHealth * 60.0f; // change unit of time : sec
		
	HealingCount = 0;
	CurrentM16BulletCount = 0;
	CurrentShotGunBulletCount = 0;
	BulletInM16 = 20;
	BulletInShotGun = 2;
	MaxShotGun = 2;
	MaxM16 = 20;

	SetMaskEffectTime = 0.0f;
	MaskTimeRemaining = 0.0f;

	MapNum = 0;
	Maplist.Add(TEXT("ForestLevel")); // MapNumber 0
	Maplist.Add(TEXT("OldHouseLevel")); // MapNumber 1

	SpawnCount = 0;
}

void ATSGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartLevel();

	RespawnHealth = CurrentHealth; // 초기 리스폰 체력을 현재 체력 값으로 설정

	GetWorldTimerManager().SetTimer( //HUD Timer
		HUDUpdateTimerHandle,
		this,
		&ATSGameState::UpdateHUD,
		0.1f, true);

	GetWorldTimerManager().SetTimer(
		BulletDataUpdateTimerHandel,
		this,
		&ATSGameState::UpdateBulletCount,
		0.1f, true);	
}

//-------------------- 리스폰 관련 함수 --------------------

// 리스폰 데이터 저장
void ATSGameState::SetRespawnPoint(const FVector& NewLocation, float NewHealth)
{
	RespawnLocation = NewLocation;
	RespawnHealth = NewHealth;
}

// 리스폰 위치 반환
FVector ATSGameState::GetRespawnLocation() const
{
	return RespawnLocation;
}

// 리스폰 체력 반환
float ATSGameState::GetRespawnHealth() const
{
	return RespawnHealth;
}
// -------------------------------------------------------------



//about Game flow
void ATSGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			TSPlayerController->ShowHUD();
		}
	}

	UpdateHUD();//임시

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(GameInstance);
		if (TSGameInstance)
		{

		}
	}
	//아이템 초기화 될 것들 변수 넣어야함
	//아이템 스포너 넣어야함

	// 몬스터 스포너
	/*TArray<AActor*> FoundEnemyVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATS_EnemySpawnVolume::StaticClass(), FoundEnemyVolumes);

	if (FoundEnemyVolumes.Num() > 0)
	{
		for (int32 i = 0; i < FoundEnemyVolumes.Num(); i++)
		{
			ATS_EnemySpawnVolume* SpawnVolume = Cast<ATS_EnemySpawnVolume>(FoundEnemyVolumes[i]);
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			for (int32 j = 0; j < SpawnCount; j++)
			{
				GetWorld()->SpawnActor<AActor>(
					Enemy,
					SpawnVolume->GetRandomPointInVolume(),
					FRotator::ZeroRotator,
					Params
				);
			}
		}
	}*/

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
			TSPlayerController->ShowGameOver();
			PopUpGameOver();

			// 죽는 애니메이션이 끝나는 3초 뒤에 SetPause(true)가 실행하도록함.
			GetWorld()->GetTimerManager().SetTimer(
				PauseForDeadAnimTimerHandle,
				FTimerDelegate::CreateWeakLambda(TSPlayerController,[TSPlayerController](){TSPlayerController->SetPause(true);}),
				1.7f,
				false
			);
		}
	}
}

void ATSGameState::EndLevel() // 스테이지 클리어
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ClearLevelNum = CurrentMapNum;
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			TSPlayerController->ShowClearScore();
			PopUpClearScore();
		}
	}
}

//void ATSGameState::EnterShelter() // 셸터 다음 레벨로 넘겨주냐 엔딩으로 보내냐
//{
//	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
//	{
//		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
//		{
//			if (Maplist.Num() > 0)
//			{
//				if (ClearLevelNum != (Maplist.Num() - 1))
//				{
//					CurrentMapNum++;
//					TSPlayerController->ShowShelterMenu();										
//					if (Maplist.IsValidIndex(CurrentMapNum))
//					{
//						OpenNextLevel();
//					}
//				}
//
//				else
//				{
//					TSPlayerController->ShowClearScore();
//				}
//			}
//		}
//	}
//	// 초기화 되는 거 초기화
//	// 중간에 스코어 표기 되나요?
//	
//}

//void ATSGameState::OpenNextLevel()
//{			
//	if (Maplist.IsValidIndex(CurrentMapNum))
//	{
//		UGameplayStatics::OpenLevel(GetWorld(), Maplist[CurrentMapNum]);
//	}		
//}

void ATSGameState::OnHPZero()
{
	OnGameOver();

	// 캐릭터의 Deaht()함수 호출로 죽는 애니메이션 재생용
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PopUpGameOver();
		// 라이플 캐릭터 죽음애니메이션처리
		ATSCharacter* PlayerCharacter = Cast<ATSCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->Death();
		}
		// 샷건 캐릭터 죽음애니메이션처리
		ATSCharacter2* PlayerCharacter2 = Cast<ATSCharacter2>(PlayerController->GetPawn());
		if (PlayerCharacter2)
		{
			PlayerCharacter2->Death();
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
				int32 HealthNum = FMath::FloorToInt(CurrentHealth);

				//1)-1 Health Text
				if (UTextBlock* HealthText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HealthText"))))
				{
					HealthText->SetText(FText::FromString(FString::Printf(TEXT("Last Time %d"), HealthNum)));
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

				// 1)-4 Health Num Text
				int32 HealthValueInt = FMath::FloorToInt(HealthValue);
				if (UTextBlock* BigHealth = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("BigHealing"))))
				{
					BigHealth->SetText(FText::FromString(FString::Printf(TEXT("+ %d"), HealthValueInt)));
				}

				if (UTextBlock* BigHealth = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("MiddleHealing"))))
				{
					BigHealth->SetText(FText::FromString(FString::Printf(TEXT("+ %d"), HealthValueInt)));
				}

				if (UTextBlock* BigHealth = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("SmallHealing"))))
				{
					BigHealth->SetText(FText::FromString(FString::Printf(TEXT("+ %d"), HealthValueInt)));
				}

				// 2) About Bullet HUD========================================================================================
				
				// 2)-1 AR Bullet
				if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))			
				{
					if (PlayerCharacter->IsA(ATSCharacter::StaticClass()))
					{
						if (UTextBlock* CountM16Bullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("M16BulletReload"))))
						{
							CountM16Bullet->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentM16BulletCount, MaxM16)));
							CountM16Bullet->SetVisibility(ESlateVisibility::Visible);
						}

						if (UTextBlock* TextM16Bullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("M16Bullet"))))
						{	
							TextM16Bullet->SetVisibility(ESlateVisibility::Visible);
						}
					}
				}
				
				// 2)-2 ShotGun Bullet
				if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
				{
					if (PlayerCharacter->IsA(ATSCharacter2::StaticClass()))
					{
						if (UTextBlock* CountShotGunBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ShotGunBulletReload"))))
						{
							CountShotGunBullet->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentShotGunBulletCount, MaxShotGun)));
							CountShotGunBullet->SetVisibility(ESlateVisibility::Visible);
						}

						if (UTextBlock* TextShotGunBullet = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ShotGun"))))
						{
							TextShotGunBullet->SetVisibility(ESlateVisibility::Visible);
						}
					}
				}
				
								
				// 3) About Gas Mask HUD========================================================================================
				if (UProgressBar* GasMask = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("Mask"))))
				{
					float GasMaskTimePercent = MaskTimeRemaining / SetMaskEffectTime;
					GasMask->SetPercent(GasMaskTimePercent);
				}
				//testmessage
				if (UTextBlock* GasMask = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("GasMasktest1"))))
				{
					float GasMaskTimePercent = MaskTimeRemaining / SetMaskEffectTime;
					GasMask->SetText(FText::FromString(FString::Printf(TEXT("left time %f MakeTime %f"), SetMaskEffectTime, MaskTimeRemaining)));
				}
				// 4) Healing Effect
				// 5) Debuff
				// 6) 피격 시 UI효과(지뢰, AI)(일시적)
				// 7) 헤드샷 UI효과

				
			}
		}
	}

}

// 2] Popup HUD

void ATSGameState::PickWidgetbyItemType(FName ItemType)
{
	EventItemType = ItemType;
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = TSPlayerController->GetHUDWidget())
			{
				TMap<FName, float> WidgetViewTime = {
					{TEXT("SmallHealing"), 3.0f},
					{TEXT("MiddleHealing"), 5.0f},
					{TEXT("BigHealing"), 7.0f},
					{TEXT("Pistol"), 7.0f},
					{TEXT("AR"), 7.0f},
					{TEXT("Mask"),30.0f}//<-이유는 모르겠지만 SetMaskEffectTime 값을 못 받아오고 있어서 수동세팅해주셔야합니다.
				};
				if (WidgetViewTime.Contains(EventItemType))
				{
					PopUpWidget(EventItemType, HUDWidget, WidgetViewTime[EventItemType]);
				}
			}
		}
	}	
	
}

TMap<FName, FTimerHandle> WidgetTimer;

void ATSGameState::PopUpWidget(FName ItemType, UUserWidget* ItemWidget, float ViewTime)
{
	if (!ItemWidget) return;

	UWidget* FoundWidget = ItemWidget->GetWidgetFromName(ItemType);
	if (!FoundWidget) return;

	FoundWidget->SetVisibility(ESlateVisibility::Visible);

	if (WidgetTimer.Contains(ItemType))
	{
		GetWorld()->GetTimerManager().ClearTimer(WidgetTimer[ItemType]);
		WidgetTimer.Remove(ItemType);
	}

	FTimerHandle NewWidgetTimerHandle;

	WidgetTimer.Add(ItemType, NewWidgetTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(
		NewWidgetTimerHandle,
		[this, FoundWidget, ItemType]()
		{
			FoundWidget->SetVisibility(ESlateVisibility::Hidden);
			WidgetTimer.Remove(ItemType);
		},
		ViewTime, false);
	
}

// 3) Clear Score UI
void ATSGameState::PopUpClearScore()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController);
		{
			if (UUserWidget* ClearScoreWidget = TSPlayerController->GetClearScoreWidget())
			{
				if (UGameInstance* GameInstance = GetGameInstance())
				{
					UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(GameInstance);
					if (TSGameInstance)
					{
						if (UTextBlock* PlayTime = Cast<UTextBlock>(ClearScoreWidget->GetWidgetFromName(TEXT("PlayTime01_data"))))
						{
							float TimeData = TSGameInstance->PlayTimeInCurrentLevel();
							int32 Minutes = FMath::FloorToInt(TimeData / 60);
							float Seconds = TimeData - (Minutes * 60);

							PlayTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds)));
						}

						if (UTextBlock* KillCount = Cast<UTextBlock>(ClearScoreWidget->GetWidgetFromName(TEXT("Killdata"))))
						{
							KillCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TSGameInstance->TotalKillCount)));
						}

						if (UTextBlock* HealCount = Cast<UTextBlock>(ClearScoreWidget->GetWidgetFromName(TEXT("Healdata"))))
						{
							HealCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TSGameInstance->TotalHealingCount)));
						}
					}
				}
			}
		}
	}
}

// 4) Game Over UI
void ATSGameState::PopUpGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController);
		{
			if (UUserWidget* GameOverWidget = TSPlayerController->GetGameOverWidget())
			{
				if (UGameInstance* GameInstance = GetGameInstance())
				{
					UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(GameInstance);
					if (TSGameInstance)
					{
						if (UTextBlock* PlayTime = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("Playtime_Count"))))
						{
							float TimeData = TSGameInstance->PlayTimeInCurrentLevel();
							int32 Minutes = FMath::FloorToInt(TimeData / 60);
							float Seconds = TimeData - (Minutes * 60);

							PlayTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds)));
						}

						if (UTextBlock* KillCount = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("KillScore_Num"))))
						{
							KillCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TSGameInstance->TotalKillCount)));
						}

						if (UTextBlock* HealCount = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("HealingScore_Num"))))
						{
							HealCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), TSGameInstance->TotalHealingCount)));
						}
					}
				}
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
	GetMaskDuration(Duration);

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

void ATSGameState::GetMaskDuration(float Value)
{
	SetMaskEffectTime = Value;
}

// 방독면 타이머 업데이트 함수 (남은 시간 반환이 아니고 업데이트 인것 같아 주석 수정했습니다. 혹시나 의도가 다르다면 연락주세요 -전보경)
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
	HealthValue = Value;
	ItemHealth += Value;
	UpdateHealth();
}

// 시간(체력) 감소함수
void ATSGameState::ReduceTime(float Value, bool bIgnoreMask)
{
	// bIgnoreMask가 true일 떄 지뢰 같은 강제 피해는 적용되도록 예외 처리
	if (bIsStopTimeReductionEnabled || bIgnoreMask)
	{
		ItemHealth -= Value ;
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

void ATSGameState::UpdateBulletCount()
{
	CurrentM16BulletCount = BulletInM16;
	CurrentShotGunBulletCount = BulletInShotGun;
}

void ATSGameState::SetM16BulletCount(int32 CurrentBullet)
{
	BulletInM16 = CurrentBullet;
}

void ATSGameState::SetShotGunBulletCount(int32 CurrentBullet)
{
	BulletInShotGun = CurrentBullet;
}

// add Kill Count
void ATSGameState::IncreaseKillCount(int32 Amount)
{
	UTSGameInstance* GameInstance = Cast<UTSGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->AddKillCount(Amount);
	}
}



