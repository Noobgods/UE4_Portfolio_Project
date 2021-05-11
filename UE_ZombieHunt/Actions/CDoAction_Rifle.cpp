#include "CDoAction_Rifle.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/ICharacter.h"
#include "Actions/CAttachment.h"
#include "Actions/CAim.h"
#include "Actors/CBullet.h"
#include "Widgets/CUserWidget_Reticle.h"

#include "GameFramework/Character.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleSystem.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Perception/AISense_Hearing.h"

ACDoAction_Rifle::ACDoAction_Rifle()
{
	PrimaryActorTick.bCanEverTick = true;

	//Ä«¸Þ¶ó ½¦ÀÌÅ©
	CHelpers::GetClass<UCameraShake>(&CameraShakeClass, "Blueprint'/Game/ActionGame/Player/BP_CameraShake.BP_CameraShake_C'");
	
	//ÃÑ¾Ë
	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/ActionGame/Weapons/BP_CBullet.BP_CBullet_C'");
	
	//ÆÄÆ¼Å¬
	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ActionGame/Particles/VFX_Muzzleflash.VFX_Muzzleflash'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/ActionGame/Particles/VFX_Eject_bullet.VFX_Eject_bullet'");
	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ActionGame/Particles/VFX_Impact_Default.VFX_Impact_Default'");
	
	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/ActionGame/Sounds/S_RifleShoot_Cue.S_RifleShoot_Cue'");
	CHelpers::GetAsset<USoundCue>(&EmptySoundCue, "SoundCue'/Game/ActionGame/Sounds/Empty_Gun_Sound_Cue.Empty_Gun_Sound_Cue'");
	
	//ÅºÈç µ¥Ä®
	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/ActionGame/Materials/MI_Decal.MI_Decal'");

	//Å©·Î½ºÇì¾î À§Á¬
	CHelpers::GetClass<UCUserWidget_Reticle>(&CrossHairClass, "WidgetBlueprint'/Game/ActionGame/Widgets/BP_CUserWidget_CrossHair.BP_CUserWidget_CrossHair_C'");

}

void ACDoAction_Rifle::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	if (!!player) {
		CrossHair = CreateWidget<UCUserWidget_Reticle, APlayerController>(player->GetController<APlayerController>(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
		CrossHair->SetSpreadSpeed(SpreadSpeed);
		CrossHair->SetReduceSpeed(ReduceSpeed);
		CrossHair->SetMinSpread(MinSpread);
		CrossHair->SetMaxSpread(MaxSpread);
	}
}

void ACDoAction_Rifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);

	CheckFalse(bAiming);

	IICharacter* rifleCharacter = Cast<IICharacter>(OwnerCharacter);
	CheckNull(rifleCharacter);

	//CrossHair Spread(Moving/Decrease)
	if (rifleCharacter->MoveSpeedRatio() > 0){
		CrossHair->UpdateWidget(true, rifleCharacter->MoveSpeedRatio());
	}
	else if (!bFiring) {
		CrossHair->UpdateWidget(false, 0);
	}
}

void ACDoAction_Rifle::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	CheckFalse(State->IsIdleMode());

	CheckFalse(bAiming);
	CheckTrue(bFiring);

	bFiring = true;
	Pitch = 0.0f;

	State->SetActionMode();

	if (bAutoFire == true) {
		GetWorld()->GetTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&ACDoAction_Rifle::Firing,
			FireRate,
			true,
			0.0f
		);

		return;
	}

	Firing();
}

void ACDoAction_Rifle::EndAction()
{
	CheckFalse(bFiring);
	bFiring = false;

	State->SetIdleMode();

	if (bAutoFire == true) {
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
	}
}

void ACDoAction_Rifle::OnAim()
{
	bAiming = true;

	Aim->On();

	CrossHair->SetVisibility(ESlateVisibility::Visible);
}

void ACDoAction_Rifle::OffAim()
{
	bAiming = false;

	Aim->Off();

	CrossHair->SetVisibility(ESlateVisibility::Hidden);

	if (bAutoFire == true) {
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
	}
}

void ACDoAction_Rifle::Begin_DoAction()
{
	Super::Begin_DoAction();
	bReloading = true;
	State->SetActionMode();
}

void ACDoAction_Rifle::End_DoAction()
{
	Super::End_DoAction();
	
	//Ammo Update
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player->GetCurrentAmmo() < (MaxAmmo-CurrentAmmo)) {
		CurrentAmmo += player->GetCurrentAmmo();
		player->SetCurrentAmmo(0);
	}
	else if (CurrentAmmo > 0) {
		player->AddCurrentAmmo(-(MaxAmmo - CurrentAmmo));
		CurrentAmmo = MaxAmmo;
	}
	else {
		player->AddCurrentAmmo(-MaxAmmo);
		CurrentAmmo = MaxAmmo;
	}
	player->UpdateAmmo(CurrentAmmo);

	bReloading = false;
	State->SetIdleMode();
}

void ACDoAction_Rifle::Reload() 
{
	//Ammo Check
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player->GetCurrentAmmo() <= 0) return;
	if (CurrentAmmo == MaxAmmo) return;

	OwnerCharacter->PlayAnimMontage(ReloadMontage, PlayRatio, StartSection);
}

void ACDoAction_Rifle::Firing()
{
	CheckTrue(bReloading);
	//--------------------------------------------------------------------------
	//Get Rifle Owner
	//--------------------------------------------------------------------------
	IICharacter* rifleCharacter = Cast<IICharacter>(OwnerCharacter);
	CheckNull(rifleCharacter);

	//--------------------------------------------------------------------------
	//Empty Ammo Check
	//--------------------------------------------------------------------------
	Mesh = rifleCharacter->GetAttachment()->GetMesh();
	if (CurrentAmmo <= 0) {
		FVector EjectLocation = Mesh->GetSocketLocation("EjectBullet");
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptySoundCue, EjectLocation);
		EndAction();
		return;
	}

	//--------------------------------------------------------------------------
	//Get Aim Ray
	//--------------------------------------------------------------------------
	FVector start;
	TArray<FVector> ray, direction;
	ray.Init(FVector(0), Shots);
	direction.Init(FVector(0), Shots);
	float spreadSize = CrossHair->GetCurrentSpread() / 50;
	for (int32 i = 0; i != Shots; i++) {
		rifleCharacter->GetAimRay(start, ray[i], direction[i], spreadSize);
	}

	//--------------------------------------------------------------------------
	//Set Etc
	//--------------------------------------------------------------------------
	CrossHair->UpdateWidget(true, 1.0f);
	CurrentAmmo--;

	//--------------------------------------------------------------------------
	//Player Setting
	//--------------------------------------------------------------------------
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player) {
		APlayerController* controller = player->GetController<APlayerController>();
		controller->PlayerCameraManager->PlayCameraShake(CameraShakeClass);

		player->UpdateAmmo(CurrentAmmo);
	}

	//--------------------------------------------------------------------------
	//Spawn Bullet
	////------------------------------------------------------------------------
	FVector muzzleLocation = Mesh->GetSocketLocation("MuzzleFlash");
	
	if (!!BulletClass) {
		for (int32 i = 0; i < Shots; i++) {
			GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction[i].Rotation());
		}
	}

	//--------------------------------------------------------------------------
	//Play Particle & SoundCue
	//--------------------------------------------------------------------------
	UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "MuzzleFlash", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "EjectBullet", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), muzzleLocation, 1.0f, OwnerCharacter, 10000.0f, "Firing_Sound");

	Pitch -= LimitPitch * GetWorld()->GetDeltaSeconds();
	if (Pitch > -LimitPitch) {
		OwnerCharacter->AddControllerPitchInput(Pitch);
	}

	//--------------------------------------------------------------------------
	//Line Trace
	//--------------------------------------------------------------------------
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(OwnerCharacter);

	FHitResult hitResult;

	//--------------------------------------------------------------------------
	//Line Trace(TakeDamage)
	//--------------------------------------------------------------------------
	for (int32 i = 0; i != Shots; i++) {
		//--------------------------------------------------------------------------
		//Line Trace(Visibility)
		//--------------------------------------------------------------------------
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, ray[i], ECollisionChannel::ECC_Visibility, params)) {
			ACharacter* hitCharacter = Cast<ACharacter>(hitResult.GetActor());
			if (!!hitCharacter) {
				//Hit Particle
				UParticleSystem* hitEffect = Datas[0].Effect;
				if (!!hitEffect) {
					FTransform transform = Datas[0].EffectTransform;
					transform.AddToTranslation(hitResult.Location);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
				}

				FPointDamageEvent pde;
				UGameplayStatics::ApplyPointDamage(hitCharacter, Datas[0].Power, direction[i], hitResult, OwnerCharacter->GetController(), this, nullptr);
				
				USkeletalMeshComponent* meshComp = CHelpers::GetComponent<USkeletalMeshComponent>(hitResult.GetActor());
				
				if (meshComp->IsSimulatingPhysics()) {
					direction[i].Normalize();
					meshComp->AddImpulseAtLocation((direction[i] * Datas[0].Power * 1000.0f), OwnerCharacter->GetActorLocation());
				}
			}
			else {
				//Render Decal
				FRotator decalRotator = hitResult.ImpactNormal.Rotation();

				UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					DecalMaterial,
					FVector(5),
					hitResult.Location,
					decalRotator, 5.0f
				);
				decal->SetFadeScreenSize(0.0f);

				//Play Impact Particle
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticle,
					hitResult.Location,
					decalRotator,
					true
				);
			}
		}

		//--------------------------------------------------------------------------
		//Line Trace(PhysicsBody)
		//--------------------------------------------------------------------------
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, ray[i], ECollisionChannel::ECC_WorldStatic, params)) {
			AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
			if (!!staticMeshActor) {
				UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
				if (!!meshComp) {
					if (meshComp->BodyInstance.bSimulatePhysics) {
						direction[i] = staticMeshActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
						direction[i].Normalize();

						meshComp->AddImpulseAtLocation(direction[i] * 3.0f, OwnerCharacter->GetActorLocation());
					}
				}
			}
		}
	}
}