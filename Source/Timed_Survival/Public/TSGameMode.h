#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "TSGameMode.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ATSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATSGameMode();

protected:
	virtual void BeginPlay() override;

	void SpawnSelectedCharacter();

	// 맵에 배치된 모든 스폰 포인트에서 아이템 생성
	void SpawnItemsFromActors();

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WeaponSelectWidgetClass;

	UUserWidget* WeaponSelectWidget;
};
