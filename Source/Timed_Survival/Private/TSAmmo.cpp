
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
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATSAmmo::HandleHit);
    RootComponent = CollisionComponent;

    
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(CollisionComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

   
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
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

void ATSAmmo::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this)
    {
        UE_LOG(LogTemp, Warning, TEXT(" HandleHit()가 올바르게 실행되지 않음!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT(" 총알이 %s 에 충돌!"), *OtherActor->GetName());

    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, FRotator::ZeroRotator);
    }

    //  데미지 적용
    float AppliedDamage = Damage;
    UE_LOG(LogTemp, Warning, TEXT(" 적용된 데미지: %f"), AppliedDamage);

    UGameplayStatics::ApplyDamage(OtherActor, AppliedDamage, GetInstigatorController(), this, UDamageType::StaticClass());

    Destroy();
}

void ATSAmmo::SetDamage(float NewDamage)
    {
        Damage = NewDamage;
    }
