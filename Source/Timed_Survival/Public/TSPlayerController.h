#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TIMED_SURVIVAL_API ATSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATSPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext; // IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction; // Move

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction; // Jump

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction; // Look

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction; // Sprint


	// UI 관련 변수, 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass; //HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass; //Main Menu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> WeaponSelectWidgetClass; //WeaponSelect Menu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* WeaponSelectWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> GameOverWidgetClass; //Game Over Menu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* GameOverWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> ClearScoreWidgetClass; // ClearScore Menu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* ClearScoreWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Opening")
	TSubclassOf<UUserWidget> OpeningWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Opening")
	UUserWidget* OpeningWidgetInstance;


	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	void StartGame();
	void ShowHUD();
	void ShowOpening();
	void ShowMainMenu();
	void ShowWeaponSelect();
	void ShowGameOver();
	void ShowClearScore();
	void QuitGame();

	virtual void BeginPlay() override;
};