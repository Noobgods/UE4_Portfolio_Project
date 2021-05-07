#include "CAnimInstance.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CDoAction_Rifle.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());
	Pitch = character->GetBaseAimRotation().Pitch;

	IICharacter* actionCharacter = Cast<IICharacter>(character);
	UCActionComponent* actionCmp = Cast<UCActionComponent>(actionCharacter->GetActionComponent());
	if (actionCmp->IsRifleMode()) {
		ACPlayer* player = Cast<ACPlayer>(actionCharacter);
		if (!!player) {
			bAiming = player->GetRightClick();
		}
	}
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
	
}
