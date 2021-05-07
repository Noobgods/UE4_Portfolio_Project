#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Ammo.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API UCUserWidget_Ammo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void OnAmmo();

	UFUNCTION(BlueprintImplementableEvent)
		void OffAmmo();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
};
