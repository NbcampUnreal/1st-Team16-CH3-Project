
#include "TSAmmo.h"
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
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATSAmmo::OnOverlap);
    RootComponent = CollisionComponent;

    
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(CollisionComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

   
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 5000.f;
    ProjectileMovement->MaxSpeed = 5000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ATSAmmo::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(10.0f);
	
}

void ATSAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

    UE_LOG(LogTemp, Warning, TEXT("총알이 %s 와(과) 오버랩!"), *OtherActor->GetName());

    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, SweepResult.Location, FRotator::ZeroRotator);
    }

    //  데미지 적용
 
    float AppliedDamage = Damage;
    UGameplayStatics::ApplyDamage(OtherActor, AppliedDamage, nullptr, this, UDamageType::StaticClass());


    //  총알 삭제
    UE_LOG(LogTemp, Warning, TEXT("총알이 오버랩 후 삭제됩니다."));
    Destroy();
}

void ATSAmmo::SetDamage(float NewDamage)
    {
        Damage = NewDamage;
    }
