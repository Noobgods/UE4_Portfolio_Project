#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemActor.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACItemActor : public AActor
{
	GENERATED_BODY()

public:
	friend class UCItem;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

private:
	UPROPERTY(EditAnywhere)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

public:
	ACItemActor();

public:
	UFUNCTION(BlueprintCallable)
		void Pick(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void Drop(int32 Amount);

public:
	FORCEINLINE UCItem* GetItem() { return Item; }
	FORCEINLINE IIItemContainer* GetContainer() { return OwnerContainer; }

protected:
	virtual void BeginPlay() override;

private:
	TArray<class UShapeComponent*> ShapeComponents;
	IIItemContainer* OwnerContainer;
	UCItem* Item;
};
