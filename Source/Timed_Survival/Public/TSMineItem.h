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
    // 폭발 감지용 콜리전
    UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Mine", meta = (AllowPrivateAccess = "true"))
    USphereComponent* ExplosionCollision;

    // 폭발 반경
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Mine")
    float ExplosionRadius;

    // 폭발 이펙트
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Effects")
    UParticleSystem* ExplosionParticle;

    // 폭발 사운드
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Effects")
    USoundBase* ExplosionSound;;

    // AI에게 줄 피해량
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Mine")
    float ExplosionAIDamage;

    // 플레이어 남은 시간 감소량
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Mine")
    float ExplosionPlayerDamage;


    // 액터가 지뢰에서 벗어났을 때 실행될 함수
    UFUNCTION()
    void HandleOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    // 폭발 실행 함수
    void Explode();

    // 폭발 여부
    bool bHasExploded;
};
