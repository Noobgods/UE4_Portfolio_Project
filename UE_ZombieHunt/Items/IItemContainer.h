#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItemContainer.generated.h"

UINTERFACE(MinimalAPI)
class UIItemContainer : public UInterface
{
	GENERATED_BODY()
};

class UE_ZOMBIEHUNT_API IIItemContainer
{
	GENERATED_BODY()

public:
	virtual void MoveItem(UObject* Container, class UCItem* Item) = 0;
	virtual void AddItem(class UCItem* Item) = 0;
};
