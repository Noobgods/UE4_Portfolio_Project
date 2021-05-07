#include "CItem.h"

void UCItem::Pick(AActor* PickCharacter)
{
}

UCItem* UCItem::Drop(int32 Amount)
{
	return nullptr;
}

void UCItem::Merge(UCItem* Item)
{
}

UCItem* UCItem::Split(int32 Amount)
{
	return nullptr;
}

void UCItem::SetQuantity(int32 Amount)
{
}

bool UCItem::AddQuantity(int32& Amount)
{
	return false;
}

bool UCItem::SubQuantity(int32& Amount)
{
	return false;
}
