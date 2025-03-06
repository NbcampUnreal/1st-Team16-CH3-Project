#include "AI/TS_EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "AI/EnemyCharacter.h"

ATS_EnemySpawnVolume::ATS_EnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(Scene);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(Scene);

}

FVector ATS_EnemySpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxOrigin = BoxComp->GetComponentLocation();
	FVector BoxExtent = BoxComp->GetScaledBoxExtent();
	return BoxOrigin + FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ATS_EnemySpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator,
		Params
	);

	return SpawnedActor;
}

