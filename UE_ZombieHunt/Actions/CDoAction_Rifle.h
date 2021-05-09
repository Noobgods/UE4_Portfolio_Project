#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Rifle.generated.h"

UCLASS()
class UE_ZOMBIEHUNT_API ACDoAction_Rifle : public ACDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		float LimitPitch = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		float FireRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		int32 Shots = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		FString UsingAmmo = "None";

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
		bool EnableAutoFire = true;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		TSubclassOf<class UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		TSubclassOf<class ACBullet> BulletClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class USoundCue* FireSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class USoundCue* EmptySoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "GunAssets")
		class UMaterialInstanceConstant* DecalMaterial;

private:
	UPROPERTY(EditDefaultsOnly, Category = "ReloadMontage")
		class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "ReloadMontage")
		float PlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "ReloadMontage")
		FName StartSection;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		TSubclassOf<class UCUserWidget_Reticle> CrossHairClass;

	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		float SpreadSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		float ReduceSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		float MinSpread;

	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		float MaxSpread;

private:
	UPROPERTY()
		class USkeletalMeshComponent* Mesh;

	UPROPERTY()
		class UCAim* Aim;

public:
	FORCEINLINE bool GetAiming() { return bAiming; }
	FORCEINLINE bool GetFiring() { return bFiring; }
	FORCEINLINE bool GetAutoFire() { return bAutoFire; }
	FORCEINLINE void ToggleAutoFire() { if(EnableAutoFire) bAutoFire = !bAutoFire; }
	FORCEINLINE int32 GetCurrentAmmo() { return CurrentAmmo; }
	FORCEINLINE FString GetUsingAmmo() { return UsingAmmo; }

public:
	ACDoAction_Rifle();

public:
	virtual void DoAction() override;
	virtual void EndAction() override;
	virtual void OnAim() override;
	virtual void OffAim() override;

	void Begin_DoAction();
	void End_DoAction();

	void Reload();

public:
	UFUNCTION()
		void Firing();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	class UCUserWidget_Reticle* CrossHair;

	FTimerHandle AutoFireTimer;
	bool bAiming;
	bool bFiring;
	bool bReloading;
	bool bAutoFire;

	float Pitch;
};
