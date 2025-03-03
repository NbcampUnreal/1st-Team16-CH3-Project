
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
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* ImpactEffect;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetDamage(float NewDamage);
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamage() const { return Damage; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;
	

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage;

public:
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }
};