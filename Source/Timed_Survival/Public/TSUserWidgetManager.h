#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TSUserWidgetManager.generated.h"

class UButton;
class UTextBlock;
class UWidgetComponent;

UCLASS()
class TIMED_SURVIVAL_API UTSUserWidgetManager : public UUserWidget
{
	GENERATED_BODY()

public:

	

	//static void AddWidget(UWidgetComponent* Widget);
	//static void RemoveWidget(UWidgetComponent* Widget);

	//static TArray<UWidgetComponent*> GetAllWidgets();

	/*void SetWidgetsShowFront();*/

	UFUNCTION()
	virtual void NativeOnInitialized() override;
};