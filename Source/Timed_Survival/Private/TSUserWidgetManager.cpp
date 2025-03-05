#include "TSUserWidgetManager.h"
#include "Kismet/GameplayStatics.h"
#include <Components/WidgetComponent.h>

void UTSUserWidgetManager::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}



//void UTSUserWidgetManager::AddWidget(UWidgetComponent* Widget)
//{
//	if (Widget)
//	{
//		ActiveWidgets.Add(Widget);
//	}
//}
//
//void UTSUserWidgetManager::RemoveWidget(UWidgetComponent* Widget)
//{
//	if (Widget)
//	{
//		ActiveWidgets.Remove(Widget);
//	}
//}
//
//TArray<UWidgetComponent*> UTSUserWidgetManager::GetAllWidgets()
//{
//	return ActiveWidgets;
//}


//void UTSUserWidgetManager::SetWidgetsShowFront()
//{
//	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
//	{
//		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
//		{
//			FVector CamaraLocation = CameraManager->GetCameraLocation();
//
//			for (UWidgetComponent* WidgetComp : ActiveWidgets )
//			{
//				FVector WidgetLocation = WidgetComp->GetComponentLocation();
//				FRotator NewRotation = (CamaraLocation - WidgetLocation).Rotation();
//				WidgetComp->SetWorldRotation(NewRotation);
//			}
//		}
//	}
//
//}

