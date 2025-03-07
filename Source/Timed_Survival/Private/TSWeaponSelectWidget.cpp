#include "TSWeaponSelectWidget.h"
#include "TSGameInstance.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "TSGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


void UTSWeaponSelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	

}

void UTSWeaponSelectWidget::SetStartWeapon(UWidget* Widget)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			if (UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				if (Widget == GetWidgetFromName(TEXT("ShotGunButton")))
				{
					if (TSubclassOf<ACharacter> ShotGunCharacter = LoadClass<ACharacter>(nullptr, TEXT("/Game/TSProject/Blueprints/BP_ShotGun_Character.BP_ShotGun_Character_C")))
					{
						TSGameInstance->SetSelectedCharacter(ShotGunCharacter);

					}
				}

				if (Widget == GetWidgetFromName(TEXT("M16Button")))
				{
					if (TSubclassOf<ACharacter> M16Character = LoadClass<ACharacter>(nullptr, TEXT("/Game/TSProject/Blueprints/BP_TS_M16_Character.BP_TS_M16_Character_C")))
					{
						TSGameInstance->SetSelectedCharacter(M16Character);

					}
				}
								
				TSPlayerController->SetInputMode(FInputModeGameOnly());
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("PlayLevel"))); //시작 맵 설정 임시
				RemoveFromParent();
			}
		}
	}
}
