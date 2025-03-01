
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSAmmo.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ATSAmmo : public AActor
{
	GENERATED_BODY()

public:
	ATSAmmo();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ImpactEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }
};