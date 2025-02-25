#include "TSGameMode.h"
#include "TSCharacter.h"
#include "TSPlayerController.h"

ATSGameMode::ATSGameMode()
{
	DefaultPawnClass = ATSCharacter::StaticClass();
	PlayerControllerClass = ATSPlayerController::StaticClass();
}