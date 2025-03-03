#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TSUserWidgetManager.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class TIMED_SURVIVAL_API UTSUserWidgetManager : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UButton* Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextBlock* TextBlock;

	UFUNCTION()
	virtual void NativeOnInitialized() override;
};