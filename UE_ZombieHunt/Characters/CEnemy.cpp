#include "CEnemy.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Actors/CItemSpawner.h"

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
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

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
			DamageValue = DamageValue * 5.0f;
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

	//Launch Enemy
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	target.Z = start.Z;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

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
	Montages->StopMontages();

	//DropRandomAmmo();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType) {
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

/* Drop Ammo
void ACEnemy::DropRandomAmmo()
{
	if (FMath::RandRange(0.0f, 10.0f) <= 1.0f) {
		FVector location = GetActorLocation();
		FRotator rotator;
		if (FMath::RandRange(0.0f, 2.0f) >= 1.0f) {
			ItemSpawner->SpawnItem(TEXT("RifleAmmo"), location, rotator, (uint32)(FMath::RandRange(20.0f, 30.0f)));
		}
		else {
			ItemSpawner->SpawnItem(TEXT("ShotgunAmmo"), location, rotator, (uint32)(FMath::RandRange(10.0f, 20.0f)));
		}
	}
}
*/