// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSItemInterface.h" // 인터페이스 헤더 파일 추가
#include "TSBaseItem.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class TIMED_SURVIVAL_API ATSBaseItem : public AActor, public ITSItemInterface // 인터페이스 상속
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATSBaseItem();

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

	// 감지 범위 (아웃라인 표시용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* DetectionSphere;

	// 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* PickupParticle;

	// 사운드(효과음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* PickupSound;

	// 아웃라인 효과용 머티리얼
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "meterial")
	UMaterialInstanceDynamic* OutlineMaterial;

	// 원래 머티리얼 (아웃라인 제거 시 사용)
	UPROPERTY()
	UMaterialInterface* OriginalMaterial;

	// 아웃라인 표시 여부 (기본값: true)
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bShowOutline;

	// 아이템 유형 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	//----------------------------------------------------------------

	// 아웃라인 효과 적용
	UFUNCTION()
	void OnDetectionOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 아웃라인 효과 해제
	UFUNCTION()
	void OnDetectionEndOverlap(
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
