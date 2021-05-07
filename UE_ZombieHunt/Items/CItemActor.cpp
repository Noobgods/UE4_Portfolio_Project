#include "CItemActor.h"

ACItemActor::ACItemActor()
{
}

void ACItemActor::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Super::BeginPlay();
}
