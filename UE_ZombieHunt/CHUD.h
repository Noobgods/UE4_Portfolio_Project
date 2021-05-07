#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetDraw() { bDraw = true; }
	FORCEINLINE void SetNotDraw() { bDraw = false; }

private:
	UPROPERTY(EditDefaultsOnly)
		FLinearColor Color;

public:
	ACHUD();

	virtual void DrawHUD() override;

	bool bDraw;
};
