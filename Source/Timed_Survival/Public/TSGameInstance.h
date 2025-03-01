#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TSGameInstance.generated.h"

UCLASS()
class TIMED_SURVIVAL_API UTSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UTSGameInstance();

	// 회복 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Food")
	int32 TotalHealingCount;

	// 회복 수집 횟수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToHealingCount(int32 Amount);

	// 선택된 캐릭터 정보 저장
	UPROPERTY(BlueprintReadWrite, Category = "Character Selection")
	TSubclassOf<ACharacter> SelectedCharacterClass;

	// 선택된 캐릭터 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	void SetSelectedCharacter(TSubclassOf<ACharacter> CharacterClass);
};
