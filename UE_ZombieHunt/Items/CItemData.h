#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItemData.generated.h"

USTRUCT()
struct FItemStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItemAttributes ItemAttribute;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACItemActor> ItemActor;
};

UCLASS()
class UE_ZOMBIEHUNT_API UCItemData : public UDataAsset
{
	GENERATED_BODY()

private:
	FString GetLabelName(ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(EditAnywhere)
		TMap<int32, FItemStruct> Items;

public:
	void TakeItemsByID(UCItem** OutItem, int32 ItemID, ACharacter* OwnerCharacter);
	void TakeItemsByName(UCItem** OutItem, FString ItemName, ACharacter* OwnerCharacter);
	void SpawnItemsByID(UCItem** OutItem, int32 ItemID, FVector Location, FRotator Rotation);
	void SpawnItemsByName(UCItem** OutItem, FString ItemName, FVector Location, FRotator Rotation);
};
