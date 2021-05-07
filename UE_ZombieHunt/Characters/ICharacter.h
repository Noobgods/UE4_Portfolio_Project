#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UE_ZOMBIEHUNT_API IICharacter
{
	GENERATED_BODY()

public:
	virtual class ACAttachment* GetAttachment() { return NULL; }
	virtual class ACDoAction* GetDoAction() { return NULL; }
	virtual class UCActionComponent* GetActionComponent() { return NULL; }

public:
	virtual void GetAimRay(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection, float SpreadSize) = 0;
	virtual float MoveSpeedRatio() = 0;

	virtual void OnFocus() {}
	virtual void OffFocus() {}
	
	virtual void UpdateAmmo(uint32 CurrentAmmo) {}

protected:
	virtual void Hitted() {}
	virtual void Dead() {}
};
