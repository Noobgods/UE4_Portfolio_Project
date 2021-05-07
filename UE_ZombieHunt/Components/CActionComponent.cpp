#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* character = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EActionType::Max; i++) {
		if (!!DataAssets[i]) {
			DataAssets[i]->BeginPlay(character, &Datas[i]);
		}
	}
}

void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type]) {
		Datas[(int32)Type]->GetEquipment()->Unequip();
	}

	Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

bool UCActionComponent::SetFistMode()
{
	return SetMode(EActionType::Fist);
}

bool UCActionComponent::SetOneHandMode()
{
	return SetMode(EActionType::OneHand);
}

bool UCActionComponent::SetRifleMode()
{
	return SetMode(EActionType::Rifle);
}

bool UCActionComponent::SetShotgunMode()
{
	return SetMode(EActionType::Shotgun);
}

bool UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return false;
	}
	else if (IsUnarmedMode() == false){
		if (!!Datas[(int32)Type]) {
			Datas[(int32)Type]->GetEquipment()->Unequip();
			SetUnarmedMode();

			return false;
		}
	}
	else if (!!Datas[(int32)InType]) {
		Datas[(int32)InType]->GetEquipment()->Equip();
		ChangeType(InType);

		return true;
	}

	return false;
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	PrevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound()) {
		OnActionTypeChanged.Broadcast(PrevType, InNewType);
	}
}

void UCActionComponent::DoAction() 
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type]) {
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action) {
			action->DoAction();
		}
	}
}

void UCActionComponent::EndAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type]) {
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action) {
			action->EndAction();
		}
	}
}

void UCActionComponent::OffAllCollisions()
{
	for (UCAction* data : Datas) {
		if (!!data == false) {
			continue;
		}

		if (!!data->GetAttachment() == false) {
			continue;
		}

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::OnAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type]) {
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action) {
			action->OnAim();
		}
	}
}

void UCActionComponent::OffAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type]) {
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action) {
			action->OffAim();
		}
	}
}