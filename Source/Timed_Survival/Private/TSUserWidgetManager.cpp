#include "TSUserWidgetManager.h"
#include "Kismet/GameplayStatics.h"
#include <Components/WidgetComponent.h>
#include "TSPlayerController.h"


void UTSUserWidgetManager::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTSUserWidgetManager::CallMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/Maps/MainMenuLevel.MainMenuLevel'")));
}

void UTSUserWidgetManager::CallGameStage()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/Maps/PlayLevel.PlayLevel'"))); // 게임 시작 맵 현재 임시로 test 맵 넣어둠
}

void UTSUserWidgetManager::QuitGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), TSPlayerController, EQuitPreference::Quit, false);
		}
	}
}
