#include "TSGameMode.h"
#include "TSCharacter.h"
#include "TSGameState.h"
#include "GameFramework/Character.h"
#include "TSGameInstance.h"
#include "TSItemSpawnPoint.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "TSPlayerController.h"

// ============================================================================================================
// 내 콘텐트 폴더에 WBP_WeaponSelect가있으면 WBP_SelectWidget에 블루프린트를 실행한다.
// 기본 PawnClass는 nullptr로 비워둔다.
// 기본 PlayerControllerClass는 ATSPlayerController::StaticClass()로 설정
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



// ============================================================================================================
// WBP_WeaponSelectWidget을 게임시작과 동시에 바로 표시한다.
void ATSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨 확인
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	// PCY_Test이면 캐릭터 스폰
	if (CurrentLevelName == "AITest")
	{
		SpawnSelectedCharacter();
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController);
		{
			TSPlayerController->ShowHUD();
		}
	}
	//else
	//{
	//	// WBP_WeaponSelect UI 표시
	//	if (WeaponSelectWidgetClass)
	//	{
	//		WeaponSelectWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponSelectWidgetClass);
	//		if (WeaponSelectWidget)
	//		{
	//			WeaponSelectWidget->AddToViewport();
	//			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//			if (PlayerController)
	//			{
	//				PlayerController->bShowMouseCursor = true;
	//				FInputModeUIOnly InputModeData;
	//				InputModeData.SetWidgetToFocus(WeaponSelectWidget->TakeWidget());
	//				PlayerController->SetInputMode(InputModeData);
	//			}
	//		}
	//	}
	//}

	SpawnItemsFromActors(); // 아이템 스폰포인트에 아이템 스폰
}



// ============================================================================================================
// 기본 PlayerStart에서 시작, 이후 리스폰지점에서 시작
void ATSGameMode::SpawnSelectedCharacter()
{

	//UTSGameInstance* GameInstance = Cast<UTSGameInstance>(GetGameInstance());
	//if (!GameInstance || !GameInstance->SelectedCharacterClass) return;

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//if (!PlayerController) return;

	//// 캐릭터 스폰 위치 지정 (임시로 (0,0,0)에서 스폰)
	//FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
	//FRotator SpawnRotation = FRotator::ZeroRotator;

	//// 캐릭터 스폰
	//ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(GameInstance->SelectedCharacterClass, SpawnLocation, SpawnRotation);
	//if (NewCharacter)
	//{
	//	// 플레이어 컨트롤러가 새로운 캐릭터를 조종하도록 설정
	//	PlayerController->Possess(NewCharacter);
	//}

	UTSGameInstance* TSGameInstance = Cast<UTSGameInstance>(GetGameInstance());
	if (!TSGameInstance || !TSGameInstance->SelectedCharacterClass) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	ATSGameState* TSGameState = GetWorld()->GetGameState<ATSGameState>();

	// 기본적으로 PlayerStart에서 가져오기
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	FVector DefaultLocation = PlayerStart ? PlayerStart->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);

	// 체크포인트가 있다면 체크포인트 위치에서 시작
	FVector SpawnLocation = (TSGameState && TSGameState->GetRespawnLocation() != FVector::ZeroVector)
		? TSGameState->GetRespawnLocation()
		: DefaultLocation;

	FRotator SpawnRotation = PlayerStart ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;

	ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(TSGameInstance->SelectedCharacterClass, SpawnLocation, SpawnRotation);
	if (NewCharacter)
	{
		PlayerController->Possess(NewCharacter);
	}
}



// ============================================================================================================
// 모든 스폰 포인트에서 아이템 스폰
void ATSGameMode::SpawnItemsFromActors()
{
	TArray<AActor*> FoundSpawnPoints;

	// 맵에 있는 모든 TSItemSpawnPoint 가져오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATSItemSpawnPoint::StaticClass(), FoundSpawnPoints);

	for (AActor* SpawnPoint : FoundSpawnPoints)
	{
		ATSItemSpawnPoint* ItemSpawn = Cast<ATSItemSpawnPoint>(SpawnPoint);
		if (ItemSpawn)
		{
			ItemSpawn->SpawnItem();
		}
	}
}

// ============================================================================================================

void ATSGameMode::SpawnEnemies()
{

}

// ============================================================================================================

// 플레이어 리스폰 함수
void ATSGameMode::RespawnPlayer(AController* PlayerController)
{
	if (!PlayerController) return;

	ATSGameState* TSGameStateRef = GetWorld()->GetGameState<ATSGameState>();
	if (!TSGameStateRef) return;

	// 저장된 리스폰 위치 가져오기
	FVector RespawnLocation = TSGameStateRef->GetRespawnLocation();
	FRotator RespawnRotation = FRotator::ZeroRotator;

	// 기존 플레이어 삭제를 위한 변수 설정
	ACharacter* OldPlayer = Cast<ACharacter>(PlayerController->GetPawn());
	TSubclassOf<ACharacter> PlayerClass = nullptr;

	if (OldPlayer && OldPlayer->ActorHasTag(TEXT("Player")))
	{
		PlayerClass = OldPlayer->GetClass(); // 기존 캐릭터 클래스 저장
		UE_LOG(LogTemp, Warning, TEXT("Destroying old player...")); // 테스트 로그 ------
		OldPlayer->Destroy(); // 기존 캐릭터 삭제
	}

	// 새로운 캐릭터 생성 (기존 클래스 유지)
	ACharacter* NewCharacter = nullptr;
	if (PlayerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning new player at: %s"), *RespawnLocation.ToString());
		NewCharacter = GetWorld()->SpawnActor<ACharacter>(PlayerClass, RespawnLocation, RespawnRotation);
	}

	// 컨트롤러 연결
	if (NewCharacter)
	{
		NewCharacter->Tags.Add(TEXT("Player"));
		PlayerController->Possess(NewCharacter);

		// 리스폰 후 GameState의 CurrentHealth를 리스폰 체력으로 설정
		TSGameStateRef->CurrentHealth = TSGameStateRef->GetRespawnHealth();
		UE_LOG(LogTemp, Warning, TEXT("Respawn successful! Health restored to: %f"), TSGameStateRef->CurrentHealth);

		//// UI 갱신
		//if (ATSPlayerController* TSPlayerController = Cast<ATSPlayerController>(PlayerController))
		//{
		//	TSPlayerController->ShowHUD();
		//}
	}
}