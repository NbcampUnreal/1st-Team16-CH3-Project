
#include "TSAmmo.h"
#include "TSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

ATSAmmo::ATSAmmo()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetGenerateOverlapEvents(true);

    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  //  모든 충돌 무시
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 적과 충돌 감지
    CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore); // 다른 총알과 충돌 방지
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);  // 벽이나 환경과 충돌 감지
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATSAmmo::OnOverlap);
    RootComponent = CollisionComponent;

    
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(CollisionComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

   
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 5000.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bSweepCollision = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ATSAmmo::BeginPlay()
{
	Super::BeginPlay();
    
    if (GetOwner())
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    }

    SetLifeSpan(5.0f);
	
}

void ATSAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATSAmmo::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}


void ATSAmmo::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("OnOverlap(): 충돌한 대상이 없음 또는 자기 자신."));
        return;
    }

    if (OtherActor->IsA(ATSCharacter::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnOverlap(): 플레이어 캐릭터와 충돌 -> 무시"));
        return;
    }

    if (OtherComp == CollisionComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnOverlap(): 자기 자신의 CollisionComponent와 충돌 -> 무시"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("총알이 %s 와(과) 오버랩!"), *OtherActor->GetName());

    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, SweepResult.Location, FRotator::ZeroRotator);
    }

    //  데미지 적용
 
    float AppliedDamage = Damage;
    UGameplayStatics::ApplyDamage(OtherActor, AppliedDamage, nullptr, this, UDamageType::StaticClass());

    UE_LOG(LogTemp, Warning, TEXT("총알이 %s 에 %f 데미지를 입힘!"), *OtherActor->GetName(), AppliedDamage);

    //  총알 삭제
    UE_LOG(LogTemp, Warning, TEXT("총알이 오버랩 후 삭제됩니다."));
    Destroy();
}
