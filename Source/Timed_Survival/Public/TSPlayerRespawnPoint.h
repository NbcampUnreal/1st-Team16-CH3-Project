// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSPlayerRespawnPoint.generated.h"

class UBoxComponent;

UCLASS()
class TIMED_SURVIVAL_API ATSPlayerRespawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ATSPlayerRespawnPoint();


protected:

	UPROPERTY(VisibleAnywhere, Category = "Respawn")
	UBoxComponent* CollisionBox;

	// 한 번 활성화된 이후 다시 발동하지 않도록 하는 변수
	bool bIsActivated;

public:

	// 스폰지점 오버랩 이벤트 함수
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
