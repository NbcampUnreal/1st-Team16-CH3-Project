// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSBaseItem.h"
#include "TSMineItem.generated.h"


class USphereComponent;
class UParticleSystem;
class USoundBase;
class ATSGameState;


UCLASS()
class TIMED_SURVIVAL_API ATSMineItem : public ATSBaseItem
{
	GENERATED_BODY()

public:
    ATSMineItem();

protected:
    // 폭발 데미지 범위로 사용할 콜리전 (예: 폭발 시 피해 적용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
    class USphereComponent* ExplosionCollision;

    // 트리거용 콜리전 컴포넌트 (지뢰 근접 영역)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
    class USphereComponent* TriggerCollision;

    // 트리거 영역의 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
    class UStaticMeshComponent* TriggerMesh;

    // 폭발 범위 (ExplosionCollision의 반경)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

	// 트리거 영역 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float TriggerRadius;

    // AI에게 줄 폭발 피해량 (HP)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionAIDamage;

    // 플레이어에게 줄 폭발 피해량 (남은 시간 감소량)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionPlayerDamage;

    // 폭발 파티클 효과
    UPROPERTY(EditDefaultsOnly, Category = "Mine")
    class UParticleSystem* ExplosionParticle;

    // 폭발 사운드 효과
    UPROPERTY(EditDefaultsOnly, Category = "Mine")
    class USoundBase* ExplosionSound;

    // 지뢰 폭발 여부
    bool bHasExploded;

    // 플레이어 또는 AI가 트리거에서 벗어나면 폭발
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) override;

    // 폭발 처리 함수
    void Explode();
};
