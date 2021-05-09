#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	//Is Hitted
	if (state->IsHittedMode()) {
		behavior->SetHittedMode();

		return;
	}

	//Is Set target
	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL) {
		behavior->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Default);

		return;
	}

	float distance = ai->GetDistanceTo(target);

	//In Behavior(MeleeAttack) Range
	if (distance < controller->GetBehaviorRange()) {
		behavior->SetActionMode();
		//controller->SetFocus(target);

		return;
	}

	//In Sight Range
	if (distance < controller->GetSightRadius()) {
		behavior->SetApproachMode();
		return;
	}
}