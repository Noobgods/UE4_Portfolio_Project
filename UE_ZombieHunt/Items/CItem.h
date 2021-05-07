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
	friend class UCItemData;

public:
	UFUNCTION(BlueprintCallable)
		void Pick(AActor* PickCharacter);

	UFUNCTION(BlueprintCallable)
		UCItem* Drop(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void Merge(UCItem* Item);

	UFUNCTION(BlueprintCallable)
		UCItem* Split(int32 Amount);

public:
	void SetQuantity(int32 Amount);

	bool AddQuantity(int32& Amount);
	bool SubQuantity(int32& Amount);

public:
	FItemAttributes* Attribute;
	ACItemActor* ActorData;

	int32 Quantity;
};
