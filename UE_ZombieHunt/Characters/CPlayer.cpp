#include "CPlayer.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction_Rifle.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CInventoryComponent.h"
#include "Items/CItem.h"
#include "Widgets/CUserWidget_Ammo.h"
#include "Widgets/CUserWidget_StatusBar.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"

//Temp
#include "Actors/CItemSpawner.h"
//Temp

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
	// Widgets Settings
	//-----------------------------------------------------------------------------
	CHelpers::GetClass<UCUserWidget_Ammo>(&WidgetAmmoClass, "WidgetBlueprint'/Game/ActionGame/Widgets/BP_CUserWidget_Ammo.BP_CUserWidget_Ammo_C'");
	CHelpers::GetClass<UCUserWidget_StatusBar>(&StatusBarClass, "WidgetBlueprint'/Game/ActionGame/Widgets/BP_CUserWidget_StatusBar.BP_CUserWidget_StatusBar_C'");

	//-----------------------------------------------------------------------------
	// Comp Settings
	//-----------------------------------------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ActionGame/Character/Paladin/paladin.paladin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/ActionGame/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
	
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->SocketOffset = FVector(0, 70, 30);
	SpringArm->TargetArmLength = 200.0;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	WidgetAmmo = CreateWidget<UCUserWidget_Ammo, APlayerController>(GetController<APlayerController>(), WidgetAmmoClass);
	WidgetAmmo->AddToViewport();
	WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);

	WidgetStatus = CreateWidget<UCUserWidget_StatusBar, APlayerController>(GetController<APlayerController>(), StatusBarClass);
	WidgetStatus->AddToViewport();
	WidgetStatus->SetVisibility(ESlateVisibility::Visible);
	WidgetStatus->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	WidgetStatus->UpdateStamina(Status->GetStamina(), Status->GetMaxStamina());

	SetGenericTeamId(TeamID);
	//Temp
	//Inventory->GetInitItems();
	//Temp
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CLog::Print(bPushForward, 1);
	CLog::Print(bPushSprint, 2);

	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + FVector(20, 0, 20);
	TArray<TEnumAsByte<EObjectTypeQuery>> type;
	type.Add(EObjectTypeQuery::ObjectTypeQuery7);
	TArray<AActor*> ignore;
	ignore.Add(this);
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, 100.0f, type, true, ignore, EDrawDebugTrace::None, hitResults, true);

	WidgetStatus->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	WidgetStatus->UpdateStamina(Status->GetStamina(), Status->GetStamina());

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Forward", EInputEvent::IE_Pressed, this, &ACPlayer::PressedForward);
	PlayerInputComponent->BindAction("Forward", EInputEvent::IE_Released, this, &ACPlayer::ReleasedForward);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::PressedSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::ReleasedSprint);

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

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);
	//Temp
	PlayerInputComponent->BindAction("TestButton", EInputEvent::IE_Pressed, this, &ACPlayer::TestButton);
	//Temp
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

void ACPlayer::PressedForward() {
	bPushForward = true;
	if (bPushForward && bPushSprint)
		OnSprint();
}

void ACPlayer::ReleasedForward() {
	bPushForward = false;
	if (!(bPushForward && bPushSprint))
		OffSprint();
}

void ACPlayer::PressedSprint()
{
	bPushSprint = true;
	if (bPushForward && bPushSprint)
		OnSprint();
}

void ACPlayer::ReleasedSprint()
{
	bPushSprint = false;
	if (!(bPushForward && bPushSprint))
		OffSprint();
}

void ACPlayer::OnSprint()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(bRightClick);

	bSprintMode = true;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OffSprint()
{
	bSprintMode = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());

	/*
	if (InputComponent->GetAxisValue("MoveForward") < 0.0f) {
		State->SetBackStepMode();
		return;
	}
	*/
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
	if (bSprintMode) OffSprint();
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
	if (bSprintMode) OffSprint();
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
	if (bSprintMode) OffSprint();
	CheckFalse(State->IsIdleMode());

	if (Action->SetRifleMode()) {
		WidgetAmmo->SetVisibility(ESlateVisibility::Visible);

		AmmoSelect();
	}
	else {
		SelectAmmo = nullptr;
		WidgetAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayer::OnShotgun()
{
	if (bRightClick) return;
	if (bSprintMode) OffSprint();
	CheckFalse(State->IsIdleMode());

	if (Action->SetShotgunMode()) {
		WidgetAmmo->SetVisibility(ESlateVisibility::Visible);

		AmmoSelect();
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
	CheckFalse(State->IsIdleMode());
	ACDoAction_Rifle* rifle = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
	if (!!rifle) {
		OffSprint();
		rifle->Reload();
	}
}

void ACPlayer::OnInteract()
{
	if (hitResults.Num() > 0) {
		ACItemActor* itemActor = Cast<ACItemActor>(hitResults[0].Actor);
		if (!!itemActor) {
			itemActor->GetContainer()->MoveItem(Inventory, itemActor->GetItem());

			AmmoSelect();
		}
	}
}


ACAttachment* ACPlayer::GetAttachment() 
{
	return Action->GetCurrent()->GetAttachment();
}

ACDoAction* ACPlayer::GetDoAction() 
{
	return Action->GetCurrent()->GetDoAction();
}

UCActionComponent* ACPlayer::GetActionComponent() 
{
	return Action;
}

UCStatusComponent* ACPlayer::GetStatusComponent()
{
	return Status;
}

void ACPlayer::AddCurrentAmmo(int Amount)
{
	SelectAmmo->AddQuantity(Amount);
}

void ACPlayer::SetCurrentAmmo(int Amount)
{
	SelectAmmo->SetQuantity(Amount);
}

int32 ACPlayer::GetCurrentAmmo()
{
	if (!!SelectAmmo) {
		return SelectAmmo->GetQuantity();
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
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

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

void ACPlayer::UpdateAmmo(uint32 CurrentAmmo)
{
	if(!!SelectAmmo)
		WidgetAmmo->UpdateAmmo(CurrentAmmo, SelectAmmo->GetQuantity());
	else
		WidgetAmmo->UpdateAmmo(CurrentAmmo, 0);
}

void ACPlayer::AmmoSelect()
{
	ACDoAction_Rifle* gun = Cast<ACDoAction_Rifle>(Action->GetCurrent()->GetDoAction());
	if (!!gun) {
		SelectAmmo = Inventory->SearchItemByName(gun->GetUsingAmmo());
		UpdateAmmo(gun->GetCurrentAmmo());
	}
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
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

void ACPlayer::OnFocus()
{
	//CrossHair->OnFocus();
}

void ACPlayer::OffFocus()
{
	//CrossHair->OffFocus();
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
		const FPointDamageEvent* pointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		FName boneName = pointDamageEvent->HitInfo.BoneName;
		if (!boneName.Compare(FName(TEXT("Head")))) {
			DamageValue = DamageValue * 100.0f;
		}
	}

	State->SetHittedMode();
	return Status->GetHealth();
}

void ACPlayer::Hitted()
{
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;

	if (Status->GetHealth() <= 0.0f) {
		State->SetDeadMode();
		return;
	}

	if (!!HitSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}

	//Play Hit Montage
	if (Action->IsRifleMode() || Action->IsShotgunMode()) {
		Montages->PlayDead();
	}
	else {
		Montages->PlayHitted();
	}
}

void ACPlayer::Dead()
{
	CheckFalse(State->IsDeadMode());
	//Montages->PlayDead();

	Action->OffAllCollisions();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	Montages->StopMontages();

}

//Temp
void ACPlayer::TestButton()
{
	FVector location = FVector(FMath::RandRange(-750.0f, 750.0f), FMath::RandRange(-750.0f, 750.0f), 110.0f);
	FRotator rotator;
	ItemSpawner->SpawnItem(TEXT("RifleAmmo"), location, rotator, 30);
}
//Temp