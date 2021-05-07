#include "CItemData.h"
#include "Global.h"
#include "CItem.h"
#include "CItemActor.h"
#include "GameFramework/Character.h"

void UCItemData::TakeItemsByID(UCItem** OutItem, int32 ItemID, ACharacter* OwnerCharacter)
{
	FTransform transform;
	ACItemActor* itemActor;
	
	FItemStruct* item = Items.Find(ItemID);
	if (!!item) {
		itemActor = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnerCharacter);
		itemActor->SetActorLabel(GetLabelName(OwnerCharacter, item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return;

	*OutItem = NewObject<UCItem>();
	(*OutItem)->Attribute = &(item->ItemAttribute);
	(*OutItem)->ActorData = itemActor;
}

void UCItemData::TakeItemsByName(UCItem** OutItem, FString ItemName, ACharacter* OwnerCharacter)
{
	FTransform transform;
	ACItemActor* itemActor;

	FItemStruct* item = nullptr;
	for (auto& i : Items) {
		if (i.Value.ItemAttribute.ItemName == ItemName) {
			item = &(i.Value);
			break;
		}
	}

	if (!!item) {
		itemActor = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnerCharacter);
		itemActor->SetActorLabel(GetLabelName(OwnerCharacter, item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return;

	*OutItem = NewObject<UCItem>();
	(*OutItem)->Attribute = &(item->ItemAttribute);
	(*OutItem)->ActorData = itemActor;
}

FString UCItemData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString name;
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}
