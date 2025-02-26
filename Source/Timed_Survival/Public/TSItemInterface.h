// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TSItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTSItemInterface : public UInterface
{
	GENERATED_BODY()
};




class TIMED_SURVIVAL_API ITSItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 아이템이 겹쳐질 때 호출되는 함수
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;

	// 벗어났을 때 호출되는 함수
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;

	// 아이템 사용 시
	virtual void ActivateItem(AActor* Activator) = 0;

	// 아이템 이름 저장
	virtual FName GetItemType() const = 0;

};
