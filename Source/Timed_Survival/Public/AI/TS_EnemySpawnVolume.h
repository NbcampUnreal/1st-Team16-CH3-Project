#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TS_EnemySpawnVolume.generated.h"

class UBoxComponent;
class AEnemyCharacter;

UCLASS()
class TIMED_SURVIVAL_API ATS_EnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ATS_EnemySpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* BoxComp;

	UFUNCTION(BlueprintCallable,Category = "Spawn")
	FVector GetRandomPointInVolume() const;
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);
};
