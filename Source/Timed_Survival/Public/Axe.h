
#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Axe.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AAxe : public ABaseWeapon
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float AttackRange;


public:
	AAxe();

	virtual void Attack() override;

	void OnHit();
};
