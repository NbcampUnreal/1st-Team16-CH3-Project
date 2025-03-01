
#pragma once

#include "CoreMinimal.h"
#include "GunWeapon.h"
#include "AR.generated.h"

UCLASS()
class TIMED_SURVIVAL_API AAR : public AGunWeapon
{
	GENERATED_BODY()
	
public:
	AAR();

	virtual void FireBullet() override;
	virtual void Reload() override;
	virtual void FinishReload() override;
};
