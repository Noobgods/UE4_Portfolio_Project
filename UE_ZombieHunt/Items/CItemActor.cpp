#include "CItemActor.h"
#include "Global.h"
#include "CItem.h"
#include "IItemContainer.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"

#include "Components/SphereComponent.h"
#include "IItemContainer.h"

ACItemActor::ACItemActor()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACItemActor::BeginPlay()
{
	OwnerContainer = Cast<IIItemContainer>(GetOwner());
	Mesh = CHelpers::GetComponent<UStaticMeshComponent>(Cast<AActor>(this));

	Super::BeginPlay();

}

void ACItemActor::Pick(AActor* InActor)
{
	SetOwner(InActor);
	if (!!GetOwner()) {
		ACPlayer* player = Cast<ACPlayer>(GetOwner());
		if (!!player && !!Mesh) {
			Mesh->SetHiddenInGame(true);
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ACItemActor::Drop(int32 Amount)
{
}