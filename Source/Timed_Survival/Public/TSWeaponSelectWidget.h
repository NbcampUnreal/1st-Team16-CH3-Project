#pragma once

#include "CoreMinimal.h"
#include "TSUserWidgetManager.h"
#include "TSWeaponSelectWidget.generated.h"

UCLASS()
class TIMED_SURVIVAL_API UTSWeaponSelectWidget : public UTSUserWidgetManager
{
	GENERATED_BODY()
	
	void NativeOnInitialized();
		
	UFUNCTION(BlueprintCallable)
	void SetStartWeapon(UWidget* Widget);

};
