
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/IItemContainer.h"
#include "CItemSpawner.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACItemSpawner : public AActor, public IIItemContainer
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCItemData* ItemDatas;

public:	
	ACItemSpawner();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void MoveItem(UObject* Container, class UCItem* Item) override;

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void AddItem(class UCItem* Item) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SpawnItem(FString ItemName, FVector Location, FRotator Rotator, int32 Quantity);

private:
	UPROPERTY()
		TArray<class UCItem*> Items;
};
