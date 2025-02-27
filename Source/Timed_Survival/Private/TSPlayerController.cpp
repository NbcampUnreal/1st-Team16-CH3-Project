#include "TSPlayerController.h"
#include "TSGameState.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

ATSPlayerController::ATSPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	ReloadAction(nullptr),
	FireAction(nullptr),
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
		UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
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
	
}
void ATSPlayerController::ShowHUD()
{
}
void ATSPlayerController::ShowOpening()
{
}
void ATSPlayerController::ShowMainMenu()
{
}
void ATSPlayerController::ShowWeaponSelect()
{
}
void ATSPlayerController::ShowGameOver()
{
}
void ATSPlayerController::ShowClearScore()
{
}
void ATSPlayerController::QuitGame()
{
}