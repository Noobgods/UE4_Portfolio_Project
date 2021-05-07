#include "CAnimNotify_BackStepped.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_BackStepped::GetNotifyName_Implementation() const
{
	return "BackStepped";
}

void UCAnimNotify_BackStepped::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_BackStep();
}