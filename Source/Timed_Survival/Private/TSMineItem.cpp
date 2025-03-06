#include "TSMineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TSGameState.h"

ATSMineItem::ATSMineItem()
{
    // 폭발 데미지 관련 초기화
    ExplosionRadius = 300.0f; // 폭발 피해 적용 범위
    TriggerRadius = 50.0f; // 트리거 영역 반지름
	ExplosionAIDamage = 30.0f; // AI에게 줄 폭발 피해량
	ExplosionPlayerDamage = 30.0f; // 플레이어에게 줄 폭발 피해량
    ItemType = "Mine";
    bHasExploded = false;
    bEnableOutline = false; // 아웃라인 표시 안함

    // 기존 ExplosionCollision: 폭발 피해 범위로 사용
    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);

    // TriggerCollision: 지뢰의 근접 영역 (예: 반지름 50)
    TriggerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollision"));
    TriggerCollision->InitSphereRadius(TriggerRadius);
    TriggerCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    TriggerCollision->SetupAttachment(Scene);
    // 트리거 콜리전에 메시 컴포넌트 연결 (트리거 영역 시각화)
    TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
    TriggerMesh->SetupAttachment(TriggerCollision);
    // 메시에는 충돌 기능이 필요없으므로 비활성화
    TriggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 트리거 영역에서 액터가 벗어날 때 HandleTriggerEndOverlap 함수 호출
    TriggerCollision->OnComponentEndOverlap.AddDynamic(this, &ATSMineItem::OnItemEndOverlap);
}

// 트리거에서 벗어나면 폭발
void ATSMineItem::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (!OtherActor || bHasExploded)
        return;

    // 플레이어나 AI가 트리거에서 벗어났다면 폭발 처리
    if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("MoveCharacter"))
    {
        Explode();
    }
}

void ATSMineItem::Explode()
{
    bHasExploded = true; // 중복 폭발 방지

    UParticleSystemComponent* ParticleComponent = nullptr;

    // 폭발 파티클 효과 생성 (한 번 재생 후 자동 제거)
    if (ExplosionParticle)
    {
        ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    if (ParticleComponent)
    {
        // 2초 후에 파티클 시스템 비활성화
        FTimerHandle ParticleDestroyTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            ParticleDestroyTimerHandle,
            [ParticleComponent]()
            {
                ParticleComponent->DeactivateSystem();
            },
            2.0f,
            false
        );
    }

    // 폭발 사운드 재생
    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            ExplosionSound,
            GetActorLocation()
        );
    }

    // ExplosionCollision을 기준으로 폭발 범위 내의 액터에게 피해 적용
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (!Actor)
            continue;

        // 플레이어의 경우, GameState에서 남은 시간을 감소
        if (Actor->ActorHasTag("Player"))
        {
            if (ATSGameState* GameState = GetWorld()->GetGameState<ATSGameState>())
            {
                GameState->ReduceTime(ExplosionPlayerDamage, true);
            }
        }
        // AI의 경우, ApplyDamage를 통해 피해 적용 --- (AI상호작용 확인 필요)
        else if (Actor->ActorHasTag("MoveCharacter"))
        {
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionAIDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // 폭발 후 지뢰 아이템 제거
    Destroy();
}

void ATSMineItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);
}