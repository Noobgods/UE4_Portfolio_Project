#include "CEnemy.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACEnemy::ACEnemy()
{
	//-----------------------------------------------------------------------------
	//Create ActionComp
	//-----------------------------------------------------------------------------
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	//-----------------------------------------------------------------------------
	// Comp Settings
	//-----------------------------------------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ActionGame/Character/Mannequin/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/ActionGame/Enemy/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

}

void ACEnemy::BeginPlay()
{
	/*
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/ActionGame/Materials/MI_Enemy_Body.MI_Enemy_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/ActionGame/Materials/MI_Enemy_Logo.MI_Enemy_Logo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	*/
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();
	
	//Action->SetUnarmedMode();

}

ACAttachment* ACEnemy::GetAttachment() {
	return Action->GetCurrent()->GetAttachment();
}

ACDoAction* ACEnemy::GetDoAction() {
	return Action->GetCurrent()->GetDoAction();
}

UCActionComponent* ACEnemy::GetActionComponent() {
	return Action;
}

float ACEnemy::MoveSpeedRatio()
{
	return GetVelocity().Size2D() / GetCharacterMovement()->MaxWalkSpeed;
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
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

void ACEnemy::Hitted()
{
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;

	if (Status->GetHealth() <= 0.0f) {
		State->SetDeadMode();
		return;
	}

	//Play Hit Montage
	Montages->PlayHitted();
	//ChangeColor(FLinearColor::Red);

	//Launch Enemy
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	target.Z = start.Z;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

	//Change Logo Material
	/*
	ChangeColor(FLinearColor::Green);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetColor", 1.0f, false);
	*/
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());
	//Montages->PlayDead();

	Action->OffAllCollisions();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);

}

/*
void ACEnemy::ChangeColor(FLinearColor InColor)
{
	InColor *= 35.0f;
	LogoMaterial->SetVectorParameterValue("LogoLight", InColor);
	LogoMaterial->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
}

void ACEnemy::ResetColor() {
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	ChangeColor(color);
}
*/

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType) {
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}
