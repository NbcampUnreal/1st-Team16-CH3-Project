// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSItemInterface.h" // 인터페이스 헤더 파일 추가
#include "TSBaseItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class TIMED_SURVIVAL_API ATSBaseItem : public AActor, public ITSItemInterface // 인터페이스 상속
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATSBaseItem();
	virtual void BeginPlay();
	// 아이템 유형 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	FName WeaponType;

protected:

	// 씬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* Scene;

	// 충돌 감지 스피어 컴포넌트 (아이템 획득 범위)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* Collision;

	// 아이템 스태틱 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMesh;
	// 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* PickupParticle;

	// 사운드(효과음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* PickupSound;
	
	//// 아이템 유형 이름
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	//FName ItemType;


	//----------------------------------------------------------------

	// 테두리 관련 변수
	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float OutlineRadius = 400.0f;  // 테두리 감지 범위

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float CustomDepthValue = 1.0f; // Custom Depth 값 (머터리얼에서 사용)

	bool bEnableOutline = true; // 기본적으로 아웃라인 적용

	// 테두리 효과 적용을 위한 트리거 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OutlineTrigger;

	//----------------------------------------------------------------

	// 플레이어가 감지 범위에 들어왔을 때 (테두리 활성화)
	UFUNCTION()
	void OnOutlineTriggerOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 플레이어가 감지 범위를 벗어났을 때 (테두리 비활성화)
	UFUNCTION()
	void OnOutlineTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);


	// 아이템이 겹쳐질 때 호출되는 함수	
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	// 벗어났을 때 호출되는 함수
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	// 아이템 활성화 시 발동 함수
	virtual void ActivateItem(AActor* Activator) override;

	// 아이템 제거
	void DestroyItem();


public:

	FName GetItemType() const;
};
