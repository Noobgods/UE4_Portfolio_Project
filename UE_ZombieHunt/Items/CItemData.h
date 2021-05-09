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
	FString GetLabelName(FString InName);

public:
	UPROPERTY(EditAnywhere)
		TMap<int32, FItemStruct> Items;

public:
	UFUNCTION(BlueprintCallable)
		class UCItem* TakeItemsByID(int32 ItemID, ACharacter* OwnerCharacter);

	UFUNCTION(BlueprintCallable)
		class UCItem* TakeItemsByName(FString ItemName, ACharacter* OwnerCharacter);

	UFUNCTION(BlueprintCallable)
		class UCItem* SpawnItemsByID(AActor* OwnActor, int32 ItemID, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
		class UCItem* SpawnItemsByName(AActor* OwnActor, FString ItemName, FVector Location, FRotator Rotation);
};
