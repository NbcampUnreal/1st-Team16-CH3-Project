
#include "Axe.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AAxe::AAxe()
{
	AttackRange = 100.0f;
	Damage = 10;
}

void AAxe::Attack()
{
	Super::Attack();

	OnHit();
}

void AAxe::OnHit()
{
    FVector Start = GetOwner()->GetActorLocation();
    FVector End = Start + GetOwner()->GetActorForwardVector() * AttackRange;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams))
    {
        if (AActor* HitActor = HitResult.GetActor())
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, GetOwner()->GetInstigatorController(), this, DamageType);
        }
    }
}
