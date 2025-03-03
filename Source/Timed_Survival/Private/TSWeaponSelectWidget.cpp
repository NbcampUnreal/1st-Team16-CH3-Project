#include "TSWeaponSelectWidget.h"
#include "TSGameInstance.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"


void UTSWeaponSelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTSWeaponSelectWidget::SetStartWeapon()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		{
			if (UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				if (GetWidgetFromName(TEXT("ShotGunButton")))
				{
					if (TSubclassOf<ACharacter> ShotGunCharacter = LoadClass<ACharacter>(nullptr, TEXT("/Game/TSProject/Blueprints/BP_ShotGun_Character")))
					{
						TSGameInstance->SetSelectedCharacter(ShotGunCharacter);
					}
				}

				if (GetWidgetFromName(TEXT("M16Button")))
				{
					if (TSubclassOf<ACharacter> M16Character = LoadClass<ACharacter>(nullptr, TEXT("/Game/TSProject/Blueprints/BP_TS_M16_Character")))
					{
						TSGameInstance->SetSelectedCharacter(M16Character);
					}
				}

				RemoveFromParent();
				TSPlayerController->SetInputMode(FInputModeGameOnly());
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("AITest"))); //시작 맵 설정 임시
			}
		}
	}
}
