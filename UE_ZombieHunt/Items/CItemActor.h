#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemActor.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACItemActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		UTexture2D* Thumbnail;

public:
	ACItemActor();

protected:
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* Mesh;
	ACharacter* OwnerCharacter;

};
