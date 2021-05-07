#include "CPlayer.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction_Rifle.h"
#include "Items/CItem.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CInventoryComponent.h"
#include "Widgets/CUserWidget_Ammo.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//-----------------------------------------------------------------------------
	//Create SceneComp
	//-----------------------------------------------------------------------------
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	//-----------------------------------------------------------------------------
	//Create ActionComp
	//-----------------------------------------------------------------------------
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Inventory, "Inventory");

	//-----------------------------------------------------------------------------
	// Comp Settings
	//-----------------------------------------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/OpenWorldGame/Character/Paladin/paladin.paladin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/ActionGame/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	CHelpers::GetClass<UCUserWidget_Ammo>(&WidgetAmmoClass, "WidgetBlueprint'/Game/ActionGame/Widgets/BP_CUserWidget_Ammo.BP_CUserWidget_Ammo_C'");

	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->SocketOffset = FVector(0, 70, 30);
	SpringArm->TargetArmLength = 200.0;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	/*
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/MI_Player_Body.MI_Player_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Player_Logo.MI_Player_Logo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	*/

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	WidgetAmmo = CreateWidget<UCUserWidget_Ammo, APlayerController>(GetController<APlayerController>(), WidgetAmmoClass);
	WidgetAmmo->AddToViewport();
	WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	
	//Temp
	Inventory->GetInitItems();
	//Temp
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("Rifle", EInputEvent::IE_Pressed, this, &ACPlayer::OnRifle);
	PlayerInputComponent->BindAction("Shotgun", EInputEvent::IE_Pressed, this, &ACPlayer::OnShotgun);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, this, &ACPlayer::OffDoAction);

	PlayerInputComponent->BindAction("AutoFire", EInputEvent::IE_Pressed, this, &ACPlayer::OnAutoFire);

	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &ACPlayer::OnReload);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0 );
	FVector direction = FQuat(rotator).GetForwardVector();
	
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());

}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f) {
		State->SetBackStepMode();

		return;
	}

	State->SetRollMode();
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	Montages->PlayBackStep();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();
}

void ACPlayer::End_BackStep()
{
	if (Action->IsUnarmedMode()) {
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	State->SetIdleMode();
}

void ACPlayer::End_Roll()
{
	if (Action->IsUnarmedMode() == false) {
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	State->SetIdleMode();
}

void ACPlayer::OnFist()
{
	if (bRightClick) return;
	CheckFalse(State->IsIdleMode());

	// ³ªÁß¿¡ Fist À§Á¬ µé¾î¿È
	if (Action->SetFistMode()) {

	}
	else {
		SelectAmmo = nullptr;
		WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnOneHand()
{
	if (bRightClick) return;
	CheckFalse(State->IsIdleMode());

	// ³ªÁß¿¡ OneHandWidget µé¾î¿È
	if (Action->SetOneHandMode()) {

	}
	else {
		SelectAmmo = nullptr;
		WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnRifle()
{
	if (bRightClick) return;
	CheckFalse(State->IsIdleMode());

	if (Action->SetRifleMode()) {
		WidgetAmmo->SetVisibility(ESlateVisibility::Visible);

		ACDoAction_Rifle* rifle = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
		if (!!rifle) {
			SelectAmmo = Inventory->SearchItemByName("RifleAmmo");
			UpdateAmmo(rifle->GetCurrentAmmo());
		}
	}
	else {
		SelectAmmo = nullptr;
		WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnShotgun()
{
	if (bRightClick) return;
	CheckFalse(State->IsIdleMode());

	if (Action->SetShotgunMode()) {
		WidgetAmmo->SetVisibility(ESlateVisibility::Visible);

		ACDoAction_Rifle* shotgun = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
		if (!!shotgun) {
			SelectAmmo = Inventory->SearchItemByName("ShotgunAmmo");
			UpdateAmmo(shotgun->GetCurrentAmmo());
		}
	}
	else {
		SelectAmmo = nullptr;
		WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::OffDoAction()
{
	Action->EndAction();
}

void ACPlayer::OnAutoFire() {
	ACDoAction_Rifle* rifle = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
	if (!!rifle) {
		rifle->ToggleAutoFire();
	}
}

void ACPlayer::OnReload() {
	ACDoAction_Rifle* rifle = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
	if (!!rifle) {
		rifle->Reload();
	}
}

ACAttachment* ACPlayer::GetAttachment() {
	return Action->GetCurrent()->GetAttachment();
}

ACDoAction* ACPlayer::GetDoAction() {
	return Action->GetCurrent()->GetDoAction();
}

UCActionComponent* ACPlayer::GetActionComponent() {
	return Action;
}

void ACPlayer::AddCurrentAmmo(int Amount)
{
	SelectAmmo->Quantity += Amount;
}

void ACPlayer::SetCurrentAmmo(int Amount)
{
	SelectAmmo->Quantity = Amount;
}

int32 ACPlayer::GetCurrentAmmo()
{
	if (!!SelectAmmo) {
		return SelectAmmo->Quantity;
	}
	else return NULL;
}

void ACPlayer::OnAim()
{
	CheckTrue(State->IsEquipMode());
	
	bRightClick = true;
	Action->OnAim();

	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();

	ZoomIn();
}

void ACPlayer::OffAim()
{
	CheckTrue(State->IsEquipMode());

	bRightClick = false;
	Action->OffAim();
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();

	ZoomOut();
}

void ACPlayer::GetAimRay(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection, float SpreadSize)
{
	OutAimDirection = Camera->GetForwardVector();

	FTransform transform = Camera->GetComponentToWorld();
	FVector cameraLocation = transform.GetLocation();
	OutAimStart = cameraLocation + OutAimDirection * 100.0f;

	FVector ConeDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OutAimDirection, SpreadSize);
	ConeDirection *= 10000.0f;

	OutAimEnd = cameraLocation + ConeDirection;
	OutAimDirection = ConeDirection;
}

void ACPlayer::OnFocus()
{
	//CrossHair->OnFocus();
}

void ACPlayer::OffFocus()
{
	//CrossHair->OffFocus();
}

void ACPlayer::UpdateAmmo(uint32 CurrentAmmo)
{
	if(!!SelectAmmo)
		WidgetAmmo->UpdateAmmo(CurrentAmmo, SelectAmmo->Quantity);
	else
		WidgetAmmo->UpdateAmmo(CurrentAmmo, 0);
}

float ACPlayer::MoveSpeedRatio()
{
	return GetVelocity().Size2D() / GetCharacterMovement()->MaxWalkSpeed;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType) {
	case EStateType::BackStep: Begin_BackStep(); break;
	case EStateType::Roll: Begin_Roll(); break;
	}
}
