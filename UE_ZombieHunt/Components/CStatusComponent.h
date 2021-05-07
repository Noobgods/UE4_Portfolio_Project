#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ZOMBIEHUNT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

private: //Scene Component
	UPROPERTY(EditAnywhere, Category = "Speed")
		float WalkSpeed = 200;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float RunSpeed = 400;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float SprintSpeed = 600;


public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed() { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE bool CanMove() { return bCanMove; }
	
public:
	UCStatusComponent();

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void SetMove();
	void SetStop();

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	bool bCanMove = true;
		
};
