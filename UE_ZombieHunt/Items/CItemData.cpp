#include "CItemData.h"
#include "Global.h"
#include "CItem.h"
#include "CItemActor.h"
#include "GameFramework/Character.h"

UCItem* UCItemData::TakeItemsByID(int32 ItemID, ACharacter* OwnerCharacter)
{
	FTransform transform;
	ACItemActor* itemActor;
	
	FItemStruct* item = Items.Find(ItemID);
	if (!!item) {
		item->ItemAttribute.ItemID = ItemID;

		itemActor = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnerCharacter);
		itemActor->SetActorLabel(GetLabelName(item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return nullptr;

	itemActor->Pick(OwnerCharacter);

	UCItem* OutItem;
	OutItem = NewObject<UCItem>();
	OutItem->SetAttribute(&(item->ItemAttribute));
	OutItem->SetActorData(itemActor);

	return OutItem;
}

UCItem* UCItemData::TakeItemsByName(FString ItemName, ACharacter* OwnerCharacter)
{
	FTransform transform;
	ACItemActor* itemActor;

	FItemStruct* item = nullptr;
	for (auto& i : Items) {
		if (i.Value.ItemAttribute.ItemName == ItemName) {
			item = &(i.Value);
			item->ItemAttribute.ItemID = i.Key;
			break;
		}
	}

	if (!!item) {
		itemActor = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnerCharacter);
		itemActor->SetActorLabel(GetLabelName(item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return nullptr;

	itemActor->Pick(OwnerCharacter);

	UCItem* OutItem;
	OutItem = NewObject<UCItem>();
	OutItem->SetAttribute(&(item->ItemAttribute));
	OutItem->SetActorData(itemActor);

	return OutItem;
}

UCItem* UCItemData::SpawnItemsByID(AActor* OwnActor, int32 ItemID, FVector Location, FRotator Rotation)
{
	FTransform transform;
	transform.SetLocation(Location);
	transform.SetRotation(Rotation.Quaternion());
	ACItemActor* itemActor;

	FItemStruct* item = Items.Find(ItemID);
	if (!!item) {
		item->ItemAttribute.ItemID = ItemID;

		itemActor = OwnActor->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnActor);
		itemActor->SetActorLabel(GetLabelName(item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return nullptr;

	UCItem* OutItem;
	OutItem = NewObject<UCItem>();
	OutItem->SetAttribute(&(item->ItemAttribute));
	OutItem->SetActorData(itemActor);

	return OutItem;
}

UCItem* UCItemData::SpawnItemsByName(AActor* OwnActor, FString ItemName, FVector Location, FRotator Rotation)
{
	FTransform transform;
	transform.SetLocation(Location);
	transform.SetRotation(Rotation.Quaternion());
	ACItemActor* itemActor;

	FItemStruct* item = nullptr;
	for (auto& i : Items) {
		if (i.Value.ItemAttribute.ItemName == ItemName) {
			item = &(i.Value);
			item->ItemAttribute.ItemID = i.Key;
			break;
		}
	}

	if (!!item) {
		itemActor = OwnActor->GetWorld()->SpawnActorDeferred<ACItemActor>(item->ItemActor, transform, OwnActor);
		itemActor->SetActorLabel(GetLabelName(item->ItemAttribute.ItemName));
		UGameplayStatics::FinishSpawningActor(itemActor, transform);
	}
	else return nullptr;

	UCItem* OutItem;
	OutItem = NewObject<UCItem>();
	OutItem->SetAttribute(&(item->ItemAttribute));
	OutItem->SetActorData(itemActor);

	return OutItem;
}


FString UCItemData::GetLabelName(FString InName)
{
	FString name;
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}