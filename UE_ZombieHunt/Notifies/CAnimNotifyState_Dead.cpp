#include "CAnimNotifyState_Dead.h"
#include "Global.h"
#include "Characters/CEnemy.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UCAnimNotifyState_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->OffAllCollisions();

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* character = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(character);

	//character->ChangeColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));

}