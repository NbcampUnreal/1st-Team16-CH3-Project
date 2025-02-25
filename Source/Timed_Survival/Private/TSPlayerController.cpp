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
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr)
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
