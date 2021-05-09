#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CItemActor.h"
#include "CItem.generated.h"

USTRUCT(BlueprintType)
struct FItemAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 ItemID;

	UPROPERTY(EditAnywhere)
		FGameplayTag AttributeTag;

	UPROPERTY(EditAnywhere)
		FString ItemName;

	UPROPERTY(EditAnywhere)
		int32 MaxQuantity;
};

UCLASS()
class UE_ZOMBIEHUNT_API UCItem : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void Merge(UCItem* Item);

	UFUNCTION(BlueprintCallable)
		UCItem* Split(int32 Amount);

public:
	FORCEINLINE int32 GetQuantity() { return Quantity; }

public:
	void SetQuantity(int32 Amount);

	void AddQuantity(int32 Amount);
	void SubQuantity(int32 Amount);

public:
	FORCEINLINE FItemAttributes* GetAttribute() { return Attribute; }
	FORCEINLINE ACItemActor* GetActorData() { return ActorData; }

public:
	void SetAttribute(FItemAttributes* InAttribute);
	void SetActorData(ACItemActor* InActorData);

private:
	FItemAttributes* Attribute;
	ACItemActor* ActorData;

	int32 Quantity;
};
