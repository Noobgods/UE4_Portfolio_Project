#include "CInventoryComponent.h"
#include "Global.h"
#include "Items/CItemData.h"
#include "Items/CItem.h"
#include "Sound/SoundCue.h"

void UCInventoryComponent::GetInitItems()
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	UCItem* item = ItemDatas->TakeItemsByName(TEXT("RifleAmmo"), character);
	if (!!item) {
		item->SetQuantity(200);
		Items.Add(item);
	}
	item = nullptr;

	item = ItemDatas->TakeItemsByName(TEXT("ShotgunAmmo"), character);
	if (!!item) {
		item->SetQuantity(100);
		Items.Add(item);
	}
}

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CLog::Print(":::::: Inventory ::::::", 3);
	int32 i = 4;
	for (auto& item : Items) {
		CLog::Print(item->GetAttribute()->ItemName + " : " + FString::FromInt(item->GetQuantity()), i++);
		if (item->GetQuantity() <= 0) {
			GetWorld()->DestroyActor(item->GetActorData());
			item->SetAttribute(nullptr);
			Items.RemoveSingle(item);
			item->ConditionalBeginDestroy();
			break;
		}
	}
}


UCItem* UCInventoryComponent::SearchItemByName(FString ItemName)
{
	for (auto item : Items) {
		if (item->GetAttribute()->ItemName == ItemName) {
			return item;
		}
	}
	return nullptr;
}

UCItem* UCInventoryComponent::SearchItemByID(int32 ItemID)
{
	return nullptr;
}

void UCInventoryComponent::MoveItem(UObject* Container, UCItem* Item)
{ 
}

void UCInventoryComponent::AddItem(UCItem* Item)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetItemCue, GetOwner()->GetActorLocation());

	for (auto item : Items) {
		if (item->GetAttribute()->ItemID == Item->GetAttribute()->ItemID) {
			if (item->GetAttribute()->MaxQuantity == item->GetQuantity()) {
				//만약에 Max를 넘겨서 아이템을 추가하고 싶다면 continue;
				return;
			}
			else if (item->GetAttribute()->MaxQuantity >= Item->GetQuantity() + item->GetQuantity()) {
				item->AddQuantity(Item->GetQuantity());
				Item->SetQuantity(0);
				return;
			}
			else if (item->GetAttribute()->MaxQuantity < Item->GetQuantity() + item->GetQuantity()) {
				item->SetQuantity(item->GetAttribute()->MaxQuantity);
				Item->SubQuantity(item->GetAttribute()->MaxQuantity - item->GetQuantity());
				return;
			}
		}
	}

	Items.Add(Item);
	Item->GetActorData()->Pick(GetOwner());
}
