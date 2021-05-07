#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ai")
		class UBehaviorTree* BehaviorTree;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	ACEnemy_AI();
};