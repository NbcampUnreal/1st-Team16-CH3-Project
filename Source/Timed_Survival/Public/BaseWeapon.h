
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class TIMED_SURVIVAL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Damage;

public:	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual int32 GetDamage() const;

};
