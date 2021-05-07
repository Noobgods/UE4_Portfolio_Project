#include "CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACEquipment::ACEquipment()
{
	
}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

void ACEquipment::Equip_Implementation() 
{
	CheckFalse(State->IsIdleMode());
	State->SetEquipMode();

	if (!!EquipData.AnimMontage) {
		OwnerCharacter->PlayAnimMontage(EquipData.AnimMontage, EquipData.PlayRatio, EquipData.StartSection);
	}
	else {
		Begin_Equip();
		End_Equip();
	}
	if (EquipData.bPawnControl == true) {
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	/*
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	character->ChangeColor(Color);
	*/
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound()) {
		OnEquipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();
}

void ACEquipment::Unequip_Implementation()
{
	CheckFalse(State->IsIdleMode());
	State->SetEquipMode();

	if (!!UnequipData.AnimMontage) {
		OwnerCharacter->PlayAnimMontage(UnequipData.AnimMontage, UnequipData.PlayRatio, UnequipData.StartSection);
	}
	else {
		Begin_Unequip();
		End_Unequip();
	}
	if (UnequipData.bPawnControl == true) {
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	/*
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	character->ChangeColor(Color);
	*/
}

void ACEquipment::Begin_Unequip_Implementation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound()) {
		OnUnequipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Unequip_Implementation()
{
	State->SetIdleMode();
}