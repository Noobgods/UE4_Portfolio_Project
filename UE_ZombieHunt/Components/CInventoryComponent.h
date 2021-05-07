#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"
#define MAX_INV 300

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ZOMBIEHUNT_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCItemData* ItemDatas;

	//Temp
public:
	void GetInitItems();
	//Temp
public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		UCItem* SearchItemByName(FString ItemName);

	UFUNCTION(BlueprintCallable)
		UCItem* SearchItemByID(int32 ItemID);

private:
	UPROPERTY()
	class UCItem* Items[MAX_INV];
};
