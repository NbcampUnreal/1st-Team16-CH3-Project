#include "TSGameInstance.h"

UTSGameInstance::UTSGameInstance()
{
	TotalHealingCount = 0;

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

//void UTSGameInstance::SetSelectedCharacter(static ConstructorHelpers::FClassFinder<AActor> CharacterClass)
//{
//	SelectedCharacterClass = CharacterClass;
//}