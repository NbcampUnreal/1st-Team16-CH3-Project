#include "TSMineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TSCharacter.h"
#include "TSGameState.h"

ATSMineItem::ATSMineItem()
{
	ExplosionRadius = 300.0f; // 폭발 반경
	ExplosionAIDamage = 30.0f; // AI에게 줄 피해량 (HP)
	ExplosionPlayerDamage = 10.0f; // 플레이어에게 줄 피해량 (시간)
    ItemType = "Mine";
	bHasExploded = false; // 폭발 여부

    // 폭발 감지용 Sphere 콜리전 설정
    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);

    // 액터가 지뢰에서 벗어날 때 실행될 함수 바인딩
    ExplosionCollision->OnComponentEndOverlap.AddDynamic(this, &ATSMineItem::HandleOverlapEnd);
}

// 지뢰에서 벗어났을 때 실행
void ATSMineItem::HandleOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (!OtherActor || bHasExploded) return;

    // 태그 기반으로 플레이어와 AI 확인
    if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("MoveCharacter"))
    {
        Explode();
    }
}

// 폭발 로직
void ATSMineItem::Explode()
{
    bHasExploded = true; // 폭발 처리 시작

    // 폭발 이펙트 생성 (한 번만 실행 후 자동 제거)
    if (ExplosionParticle)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    // 폭발 사운드
    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            ExplosionSound,
            GetActorLocation()
        );
    }

    // 폭발 범위 내의 액터 탐색
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (!Actor) continue;

        if (Actor->ActorHasTag("Player"))
        {
            // 플레이어의 경우 남은 시간 감소
            if (ATSGameState* GameState = GetWorld()->GetGameState<ATSGameState>())
            {
                GameState->ReduceTime(ExplosionPlayerDamage);
            }
        }
        else if (Actor->ActorHasTag("MoveCharacter"))
        {
            // AI의 경우 HP 감소
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionAIDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // 지뢰 즉시 제거
    Destroy();
}
