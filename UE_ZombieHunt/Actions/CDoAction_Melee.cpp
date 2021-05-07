#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	if (bEnable == true) {
		bExist = true;
		bEnable = false;

		return;
	}

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;

	OwnerCharacter->StopAnimMontage();

	Index++;

	FMath::Clamp<int32>(Index, 0, (int32)EStateType::Max - 1);

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);
	Index = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOtherCharacter);

	//Block Multi Hitting
	for (const ACharacter* other : HittedCharacter) {
		if (other == InOtherCharacter) {
			return;
		}
	}
	HittedCharacter.Add(InOtherCharacter);

	//Hit Stop
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalTime", hitStop * 2e-2f, false);
	}

	//Hit Particle
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect) {
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;
	if (!!shake) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}

	//Taking Damage
	FDamageEvent e;
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackerCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackerCauser, InOtherCharacter);

	HittedCharacter.Empty();
}

void ACDoAction_Melee::ResetGlobalTime()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);

}