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

	
	UFUNCTION()
	virtual void NativeOnInitialized() override;
};