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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* TextBlock;

	virtual void NativeOnInitialized() =0; //UMG 생성자
}
