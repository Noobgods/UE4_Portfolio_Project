#include "CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACDoAction::ACDoAction()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ACDoAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();

}

void ACDoAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

