#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/IItemContainer.h"
#include "CInventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ZOMBIEHUNT_API UCInventoryComponent : public UActorComponent, public IIItemContainer
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCItemData* ItemDatas;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* GetItemCue;

	//Temp
public:
	void GetInitItems();
	//Temp
public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION(BlueprintCallable)
		UCItem* SearchItemByName(FString ItemName);

	UFUNCTION(BlueprintCallable)
		UCItem* SearchItemByID(int32 ItemID);

private:
	UPROPERTY()
		TArray<class UCItem*> Items;

public:
	virtual void MoveItem(UObject* Container, class UCItem* Item) override;
	virtual void AddItem(class UCItem* Item) override;

};
