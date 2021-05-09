#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Reticle.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API UCUserWidget_Reticle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void IncreaseReticleSpread();

	UFUNCTION(BlueprintImplementableEvent)
		void DecreaseReticleSpread();

	UFUNCTION(BlueprintCallable)
		void SetReticleTranslation(UWidget* Target, float TranslationX, float TranslationY, float InterpSpeed);

public:
	FORCEINLINE float GetCurrentSpread() { return CurrentSpread; }

public:
	void UpdateWidget(bool OnSpread, int Scale);
	void SetMinSpread(float Min) { MinSpread = Min; }
	void SetMaxSpread(float Max) { MaxSpread = Max; }
	void SetSpreadSpeed(float Speed) { SpreadSpeed = Speed;  }
	void SetReduceSpeed(float Speed) { ReduceSpeed = Speed; }

private:
	float GetReticleMinSpread(float Scale);
	float GetReticleMaxSpread(float Scale);

protected :
	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float MinSpread;

	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float MaxSpread;

	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float SpreadSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float ReduceSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Reticle")
		float CurrentSpread;
};
