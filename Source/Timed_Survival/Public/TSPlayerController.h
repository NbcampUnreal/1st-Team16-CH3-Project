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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction; // Crouch

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction; // Reload

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction; // Fire

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AimingAction;

	// about UI Function
	FString CurrentMapName;

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
	TSubclassOf<UUserWidget> OpeningWidgetClass; // Opening
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Opening")
	UUserWidget* OpeningWidgetInstance;


	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHUD();
	UFUNCTION(BlueprintCallable, Category = "Opening")
	void ShowOpening();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowWeaponSelect();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOver();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowClearScore();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ClearWidget(); // Clear All Widget except HUD
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void QuitGame();

	virtual void BeginPlay() override;
};