#include "CHUD.h"
#include "Global.h"
#include "Widgets/CUserWidget_Ammo.h"

ACHUD::ACHUD()
{
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();
	CheckFalse(bDraw);

}