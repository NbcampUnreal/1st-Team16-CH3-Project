#include "TSPlayerController.h"
#include "TSGameState.h"
#include "TSGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
	OpeningWidgetClass(nullptr),
	OpeningWidgetInstance(nullptr)
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
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	ATSGameState* TSGameState = GetWorld() ? GetWorld()->GetGameState<ATSGameState>() : nullptr;
	if (TSGameState)
	{
		TSGameState->UpdateHUD();
	}
}

UUserWidget* ATSPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ATSPlayerController::StartGame()
{
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

	ClearWidget();

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

void ATSPlayerController::QuitGame()
{

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
}