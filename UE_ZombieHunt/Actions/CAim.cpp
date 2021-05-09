#include "CAim.h"
#include "Global.h"
#include "CHud.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"

UCAim::UCAim()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/ActionGame/Actions/Curve_Aim.Curve_Aim'");
}

void UCAim::BeginPlay(class ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	TimelineFloat.BindUFunction(this, "Zooming");
	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200);

	Hud = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();
}

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(IsAvailable());
	CheckTrue(bInZoom);

	bInZoom = true;

	Hud->SetDraw();
	Timeline.PlayFromStart();

	//SpringArm->TargetArmLength = 200.0;
	SpringArm->SocketOffset = FVector(0, 50, 30);
	//SpringArm->bEnableCameraLag = false;

}

void UCAim::Off()
{
	CheckFalse(IsAvailable());
	CheckFalse(bInZoom);

	bInZoom = false;

	Hud->SetNotDraw();
	Timeline.ReverseFromEnd();

	//SpringArm->TargetArmLength = 200.0;
	SpringArm->SocketOffset = FVector(0, 70, 30);
	//SpringArm->bEnableCameraLag = true;
}

void UCAim::Zooming(float Value)
{
	Camera->FieldOfView = Value;
}