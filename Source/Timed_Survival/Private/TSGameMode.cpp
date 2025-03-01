#include "TSGameMode.h"
#include "TSCharacter.h"
#include "TSGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TSPlayerController.h"

ATSGameMode::ATSGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponSelectWidgetBP(TEXT("/Game/UI/Widget/WBP_WeaponSelect.WBP_WeaponSelect"));
	if (WeaponSelectWidgetBP.Succeeded())
	{
		WeaponSelectWidgetClass = WeaponSelectWidgetBP.Class;
	}

	DefaultPawnClass = nullptr; // 기본 캐릭터 없음 (선택된 캐릭터를 스폰할 것이므로)
	PlayerControllerClass = ATSPlayerController::StaticClass();
}

void ATSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨 확인
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	// PCY_Test이면 캐릭터 스폰
	if (CurrentLevelName == "PCY_Test")
	{
		SpawnSelectedCharacter();
	}
	else
	{
		// WBP_WeaponSelect UI 표시
		if (WeaponSelectWidgetClass)
		{
			WeaponSelectWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponSelectWidgetClass);
			if (WeaponSelectWidget)
			{
				WeaponSelectWidget->AddToViewport();
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->bShowMouseCursor = true;
					FInputModeUIOnly InputModeData;
					InputModeData.SetWidgetToFocus(WeaponSelectWidget->TakeWidget());
					PlayerController->SetInputMode(InputModeData);
				}
			}
		}
	}
}

void ATSGameMode::SpawnSelectedCharacter()
{
	UTSGameInstance* GameInstance = Cast<UTSGameInstance>(GetGameInstance());
	if (!GameInstance || !GameInstance->SelectedCharacterClass) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	// 캐릭터 스폰 위치 지정 (임시로 (0,0,300)에서 스폰)
	FVector SpawnLocation = FVector(0.0f, 0.0f, 300.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 캐릭터 스폰
	ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(GameInstance->SelectedCharacterClass, SpawnLocation, SpawnRotation);
	if (NewCharacter)
	{
		// 플레이어 컨트롤러가 새로운 캐릭터를 조종하도록 설정
		PlayerController->Possess(NewCharacter);
	}
}
