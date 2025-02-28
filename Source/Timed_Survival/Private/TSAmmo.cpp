
#include "TSAmmo.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

ATSAmmo::ATSAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATSAmmo::OnHit);
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

void ATSAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("The bullet hit %s!"), *OtherActor->GetName());

        UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass());
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, FRotator::ZeroRotator);

        Destroy();
    }
}