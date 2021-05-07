#include "CInventoryComponent.h"
#include "Global.h"
#include "Items/CItemData.h"
#include "Items/CItem.h"

void UCInventoryComponent::GetInitItems()
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	ItemDatas->TakeItemsByName(&Items[0], TEXT("RifleAmmo"), character);
	if (!!Items[0]) {
		Items[0]->Quantity = 200;
		CLog::Print(Items[0]->Attribute->ItemName);
	}

	ItemDatas->TakeItemsByName(&Items[1], TEXT("ShotgunAmmo"), character);
	if (!!Items[1]) {
		Items[1]->Quantity = 100;
		CLog::Print(Items[1]->Attribute->ItemName);
	}
}

UCInventoryComponent::UCInventoryComponent()
{
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

UCItem* UCInventoryComponent::SearchItemByName(FString ItemName)
{
	for (int i = 0; i < MAX_INV; i++) {
		if (!!Items[i]) {
			if (ItemName == Items[i]->Attribute->ItemName) {
				CLog::Print(Items[i]->Attribute->ItemName);
				return Items[i];
			}
		}
	}
	return nullptr;
}

UCItem* UCInventoryComponent::SearchItemByID(int32 ItemID)
{
	return nullptr;
}