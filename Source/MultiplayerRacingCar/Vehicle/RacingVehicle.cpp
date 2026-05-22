// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingVehicle.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

ARacingVehicle::ARacingVehicle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	IdleAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("IdleAudioComponent"));
	IdleAudioComponent->SetupAttachment(GetRootComponent());
	IdleAudioComponent->bAutoActivate = false;

	AccelerateAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AccelerateAudioComponent"));
	AccelerateAudioComponent->SetupAttachment(GetRootComponent());
	AccelerateAudioComponent->bAutoActivate = false;

	EngineAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudioComponent"));
	EngineAudioComponent->SetupAttachment(GetRootComponent());
	EngineAudioComponent->bAutoActivate = false;
}

void ARacingVehicle::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		Multicast_PlaySounds();
	}
	else
	{
		Server_RequestPlaySounds();
	}
}

void ARacingVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSound(DeltaTime);
}

void ARacingVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARacingVehicle, IdleSound);
	DOREPLIFETIME(ARacingVehicle, AccelerateSound);
	DOREPLIFETIME(ARacingVehicle, EngineSound);
}


void ARacingVehicle::UpdateSound(float DeltaTime)
{
	if (!EngineAudioComponent || !AccelerateAudioComponent || !IdleAudioComponent) return;
	float CurrentSpeed = GetVelocity().Size() * 0.0223694f;
	float TargetIdleSOundVolume = (CurrentSpeed < 10.f) ? 1.f : 0.f;
	float TargetEngineSoundVoulme = (CurrentSpeed >= 10.f) ? 1.f : 0.f;

	float IdleVolume = FMath::FInterpTo(IdleAudioComponent->VolumeMultiplier, TargetIdleSOundVolume, DeltaTime, 3.f);
	float EngineVolume = FMath::FInterpTo(EngineAudioComponent->VolumeMultiplier, TargetEngineSoundVoulme, DeltaTime, 3.f);
	IdleAudioComponent->SetVolumeMultiplier(IdleVolume);
	EngineAudioComponent->SetVolumeMultiplier(EngineVolume);

	float TargetAccelerateSoundVolume = bIsAccelerating ? 1.5f : 0.f;
	float AccelerateVolume = FMath::FInterpTo(AccelerateAudioComponent->VolumeMultiplier, TargetAccelerateSoundVolume, DeltaTime, 3.f);
	AccelerateAudioComponent->SetVolumeMultiplier(AccelerateVolume);
}

void ARacingVehicle::Server_RequestPlaySounds_Implementation()
{
	Multicast_UpdateAllClients();
}
void ARacingVehicle::Multicast_PlaySounds_Implementation()
{
	if (IdleSound)
	{
		IdleAudioComponent->SetSound(IdleSound);
		IdleAudioComponent->Play();
		IdleAudioComponent->SetVolumeMultiplier(1.f);
	}
	if (AccelerateSound)
	{
		AccelerateAudioComponent->SetSound(AccelerateSound);
		AccelerateAudioComponent->Play();
		AccelerateAudioComponent->SetVolumeMultiplier(0.f);
	}
	if (EngineSound)
	{
		EngineAudioComponent->SetSound(EngineSound);
		EngineAudioComponent->Play();
		EngineAudioComponent->SetVolumeMultiplier(0.f);
	}
}
void ARacingVehicle::Multicast_UpdateAllClients_Implementation()
{
	for (TActorIterator<ARacingVehicle> IT(GetWorld()); IT; ++IT)
	{
		ARacingVehicle* Vehicle = *IT;
		if (Vehicle)
		{
			Vehicle->Multicast_PlaySounds();
		}
	}
}