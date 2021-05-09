#include "CItemSpawner.h"
#include "Global.h"
#include "Items/CItem.h"
#include "Items/CItemData.h"

ACItemSpawner::ACItemSpawner()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ACItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	FVector location(50.0f, 730.0, 110.0f);
	FRotator rotator(0, 0, 0);
	UCItem* item = ItemDatas->SpawnItemsByName(this, TEXT("RifleAmmo"), location, rotator);
	if (!!item) {
		item->SetQuantity(200);
		Items.Add(item);
	}
	item = nullptr;

	location = FVector(-180.0f, 730.0, 110.0f);
	rotator = FRotator(0, 0, 0);
	item = ItemDatas->SpawnItemsByName(this, TEXT("ShotgunAmmo"), location, rotator);
	if (!!item) {
		item->SetQuantity(100);
		Items.Add(item);
	}
}

void ACItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItemSpawner::MoveItem(UObject* Container, UCItem* Item)
{
	IIItemContainer* inventory = Cast<IIItemContainer>(Container);

	inventory->AddItem(Item);

	if (Item->GetQuantity() <= 0) {
		for (auto& item : Items) {
			if (item == Item) {
				GetWorld()->DestroyActor(item->GetActorData());
				item->SetAttribute(nullptr);
				Items.RemoveSingle(item);
				item->ConditionalBeginDestroy();
				break;
			}
		}
	}
	if (Item->GetActorData()->GetContainer() != this) {
		for (auto& item : Items) {
			if (item == Item) {
				Items.RemoveSingle(item);
				break;
			}
		}
	}
}

void ACItemSpawner::AddItem(UCItem* Item)
{
}

void ACItemSpawner::SpawnItem(FString ItemName, FVector Location, FRotator Rotator, uint32 Quantity = 1)
{
	UCItem* item = ItemDatas->SpawnItemsByName(this, ItemName, Location, Rotator);
	if (!!item) {
		item->SetQuantity(Quantity);
		Items.Add(item);
	}
}

