
#pragma once

#include "CoreMinimal.h"
#include "GunWeapon.h"
#include "Pistol.generated.h"


UCLASS()
class TIMED_SURVIVAL_API APistol : public AGunWeapon
{
	GENERATED_BODY()
	
public:
	APistol();

	virtual void FireBullet() override;
	virtual void Reload() override;
	virtual void FinishReload() override;
};
