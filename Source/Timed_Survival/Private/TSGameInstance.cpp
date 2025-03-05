#include "TSGameInstance.h"

UTSGameInstance::UTSGameInstance()
{
	TotalHealingCount = 0;
	TotalKillCount = 0;

	SelectedCharacterClass = nullptr;

}

// 회복 수집 횟수
void UTSGameInstance::AddToHealingCount(int32 Amount)
{
	TotalHealingCount += Amount;
}

void UTSGameInstance::SetSelectedCharacter(TSubclassOf<ACharacter> CharacterClass)
{
	SelectedCharacterClass = CharacterClass;
}

void UTSGameInstance::StoreStartTime()
{
	LevelStartTime = GetWorld()->TimeSeconds;
}

float UTSGameInstance::PlayTimeInCurrentLevel() const
{
	return GetWorld()->TimeSeconds - LevelStartTime;
}

void UTSGameInstance::AddKillCount(int32 Amount)
{
	TotalKillCount += Amount;
}

