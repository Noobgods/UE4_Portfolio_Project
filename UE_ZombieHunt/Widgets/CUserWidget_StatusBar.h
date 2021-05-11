#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_StatusBar.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API UCUserWidget_StatusBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStamina(float InStamina, float InMaxStamina);
};
