#include "TSWeaponSelectWidget.h"
#include "TSGameInstance.h"
#include "TSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


void UTSWeaponSelectWidget::NativeOnInitialized()
{
	Button->OnClicked;	
}

void UTSWeaponSelectWidget::SetStartWeapon()
{	
	//static ConstructorHelpers::FClassFinder<AActor> M16Character(TEXT("/Game/TSProject/Blueprints/BP_TS_M16_Character"));
	//static ConstructorHelpers::FClassFinder<AActor> ShotGunCharacter(TEXT("/Game/TSProject/Blueprints/BP_ShotGun_Character"));
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
						RemoveFromParent();
						TSPlayerController->SetInputMode(FInputModeGameOnly());
						UGameplayStatics::OpenLevel(GetWorld(),FName(TEXT("AITest"))); //시작 맵 설정 임시
					}
				}

				if (GetWidgetFromName(TEXT("M16Button")))
				{
					if (TSubclassOf<ACharacter> M16Character = LoadClass<ACharacter>(nullptr, TEXT("/Game/TSProject/Blueprints/BP_TS_M16_Character")))
					{
						TSGameInstance->SetSelectedCharacter(M16Character);
						RemoveFromParent();
						TSPlayerController->SetInputMode(FInputModeGameOnly());
						UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("AITest"))); //시작 맵 설정 임시
					}
				}
			}
		}
	}
