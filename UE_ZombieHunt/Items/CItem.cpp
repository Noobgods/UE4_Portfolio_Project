#include "CItem.h"
#include "Global.h"

void UCItem::Merge(UCItem* Item)
{
}

UCItem* UCItem::Split(int32 Amount)
{
	return nullptr;
}

void UCItem::SetQuantity(int32 Amount)
{
	Quantity = Amount;
}

void UCItem::AddQuantity(int32 Amount)
{
	Quantity += Amount;
}

void UCItem::SubQuantity(int32 Amount)
{
	Quantity -= Amount;
}

void UCItem::SetAttribute(FItemAttributes* InAttribute) 
{ 
	Attribute = InAttribute; 
}

void UCItem::SetActorData(ACItemActor* InActorData) 
{ 
	ActorData = InActorData; 
	ActorData->Item = this;
}