
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
	void HandleHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ImpactEffect;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetDamage(float NewDamage);
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamage() const { return Damage; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage;

public:
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }
};