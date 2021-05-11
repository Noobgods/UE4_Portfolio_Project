#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;

private: //Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<class ACItemSpawner> ItemSpawner;

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual class ACAttachment* GetAttachment();
	virtual class ACDoAction* GetDoAction();
	virtual class UCActionComponent* GetActionComponent();

public:
	virtual void GetAimRay(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection, float SpreadSize) {};

	virtual float MoveSpeedRatio();

	virtual void OnFocus() override {};
	virtual void OffFocus() override {};

public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

private:
	virtual void Hitted();
	virtual void Dead();

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
	//void DropRandomAmmo();

private:
	class AController* DamageInstigator;
	float DamageValue;

};
