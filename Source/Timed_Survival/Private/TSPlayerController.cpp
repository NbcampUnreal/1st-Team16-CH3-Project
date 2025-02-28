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