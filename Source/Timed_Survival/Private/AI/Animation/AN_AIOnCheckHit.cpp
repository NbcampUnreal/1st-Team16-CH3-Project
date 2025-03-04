#include "AI/Animation/AN_AIOnCheckHit.h"
#include "AI/EnemyCharacter.h"

void UAN_AIOnCheckHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp) == true)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (IsValid(Enemy) == true)
		{
			Enemy->OnCheckHit();
		}
	}
}
