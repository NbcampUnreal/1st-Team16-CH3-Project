#include "TSPlayerController.h"
#include "TSGameState.h"
#include "TSGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TSGameMode.h"

ATSPlayerController::ATSPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	CrouchAction(nullptr),
	ReloadAction(nullptr),
	FireAction(nullptr),
	AimingAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	WeaponSelectWidgetClass(nullptr),
	WeaponSelectWidgetInstance(nullptr),
	GameOverWidgetClass(nullptr),
	GameOverWidgetInstance(nullptr),
	ClearScoreWidgetClass(nullptr),
	ClearScoreWidgetInstance(nullptr),
	ShelterMenuWidgetClass(nullptr),
	ShelterMenuWidgetInstance(nullptr),
	OpeningWidgetClass(nullptr),
	OpeningWidgetInstance(nullptr),
	ShotEventWidgetClass(nullptr),
	ShotEventWidgetInstance(nullptr),
	AimWidgetClass(nullptr),
	AimWidgetInstance(nullptr)
{
}

void ATSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	//UI
	CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MainMenuLevel"))
	{
		ShowMainMenu();
	}
}


UUserWidget* ATSPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UUserWidget* ATSPlayerController::GetClearScoreWidget() const
{
	return ClearScoreWidgetInstance;
}

UUserWidget* ATSPlayerController::GetGameOverWidget() const
{
	return GameOverWidgetInstance;
}

void ATSPlayerController::StartGame()
{
	ChangeToIMC();

	if (UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		TSGameInstance->TotalHealingCount = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("ForestLevel"));
}

void ATSPlayerController::ShowHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	ClearWidget();
	
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ATSGameState* TSGameState = GetWorld() ? GetWorld()->GetGameState<ATSGameState>() : nullptr;
			if (TSGameState)
			{
				TSGameState->UpdateHUD();
			}
		}
	}

	ShowAimWidget();
}

void ATSPlayerController::ShowOpening()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	ClearWidget();
}

void ATSPlayerController::ShowMainMenu()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	ClearWidget();
	ChangeToWidgetIMC();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATSPlayerController::ShowWeaponSelect()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	//ClearWidget();
	ChangeToWidgetIMC();

	if (WeaponSelectWidgetClass)
	{
		WeaponSelectWidgetInstance = CreateWidget<UUserWidget>(this, WeaponSelectWidgetClass);
		if (WeaponSelectWidgetInstance)
		{
			WeaponSelectWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATSPlayerController::ShowGameOver()
{
	ClearWidget();
	ChangeToWidgetIMC();

	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATSPlayerController::ShowClearScore()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	ClearWidget();
	ChangeToWidgetIMC();

	if (ClearScoreWidgetClass)
	{
		ClearScoreWidgetInstance = CreateWidget<UUserWidget>(this, ClearScoreWidgetClass);
		if (ClearScoreWidgetInstance)
		{
			ClearScoreWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
void ATSPlayerController::ShowShelterMenu()
{
	ClearWidget();
	ChangeToWidgetIMC();

	if (ShelterMenuWidgetClass)
	{
		ShelterMenuWidgetInstance = CreateWidget<UUserWidget>(this, ShelterMenuWidgetClass);
		if (ShelterMenuWidgetInstance)
		{
			ShelterMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATSPlayerController::ShowShotEventWidget()
{
	if (ShotEventWidgetClass)
	{
		ShotEventWidgetInstance = CreateWidget<UUserWidget>(this, ShotEventWidgetClass);
		if (ShotEventWidgetInstance)
		{
			ShotEventWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void ATSPlayerController::ShowAimWidget()
{
	if (AimWidgetClass)
	{
		AimWidgetInstance = CreateWidget<UUserWidget>(this, AimWidgetClass);
		if (AimWidgetInstance)
		{
			AimWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void ATSPlayerController::QuitGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), TSPlayerController, EQuitPreference::Quit, false);
		}
	}
}

void ATSPlayerController::ClearWidget() // Clear All Widget except HUD
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (WeaponSelectWidgetInstance)
	{
		WeaponSelectWidgetInstance->RemoveFromParent();
		WeaponSelectWidgetInstance = nullptr;
	}

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
	}

	if (ClearScoreWidgetInstance)
	{
		ClearScoreWidgetInstance->RemoveFromParent();
		ClearScoreWidgetInstance= nullptr;
	}

	if (ShelterMenuWidgetInstance)
	{
		ShelterMenuWidgetInstance->RemoveFromParent();
		ShelterMenuWidgetInstance = nullptr;
	}

	if (ShotEventWidgetInstance)
	{
		ShotEventWidgetInstance->RemoveFromParent();
		ShotEventWidgetInstance = nullptr;
	}
	
	if (AimWidgetInstance)
	{
		AimWidgetInstance->RemoveFromParent();
		AimWidgetInstance = nullptr;
	}
}

void ATSPlayerController::ChangeToWidgetIMC()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->RemoveMappingContext(InputMappingContext);
				Subsystem->AddMappingContext(WidgetInputMappingContext, 1);
			}

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATSPlayerController::ChangeToIMC()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (WidgetInputMappingContext)
			{
				Subsystem->RemoveMappingContext(WidgetInputMappingContext);
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

// 리스폰 버튼을 누르면 호출되는 함수
void ATSPlayerController::RequestRespawn()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn Button Pressed!"));

	ATSGameMode* GameMode = Cast<ATSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->RespawnPlayer(this);
	}
}