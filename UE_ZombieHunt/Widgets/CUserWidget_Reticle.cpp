#include "CUserWidget_Reticle.h"
#include "Global.h"

void UCUserWidget_Reticle::SetReticleTranslation(UWidget* Target, float TranslationX, float TranslationY, float InterpSpeed)
{
	FVector2D current = Target->RenderTransform.Translation;
	FVector2D target = FVector2D(TranslationX, TranslationY);
	FVector2D translation = UKismetMathLibrary::Vector2DInterpTo(current, target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
	Target->SetRenderTranslation(translation);
}

void UCUserWidget_Reticle::UpdateWidget(bool OnSpread, int Scale)
{
	if (OnSpread) {
		float spread = GetReticleMaxSpread(Scale);
		CurrentSpread = spread > CurrentSpread ? spread : CurrentSpread;
		IncreaseReticleSpread();
	}
	else {
		CurrentSpread = GetReticleMinSpread(Scale);
		DecreaseReticleSpread();
	}
}

float UCUserWidget_Reticle::GetReticleMinSpread(float Scale)
{
	return UKismetMathLibrary::FClamp(MinSpread * Scale, MinSpread, MaxSpread);
}

float UCUserWidget_Reticle::GetReticleMaxSpread(float Scale)
{
	return UKismetMathLibrary::FClamp(MaxSpread * Scale + MinSpread, MinSpread, MaxSpread);
}