// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "RacingVehicle.generated.h"


class UGameEndAnnouncement;
class UAudioComponent;
class USoundBase;
/**
 * 
 */
UCLASS()
class MULTIPLAYERRACINGCAR_API ARacingVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	ARacingVehicle();

	UPROPERTY(EditAnywhere, Category="WinnerAnnouncement")
	TObjectPtr<UGameEndAnnouncement> GameAnnouncement;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(EditAnywhere, Category = "Audio Components")
	TObjectPtr<UAudioComponent> IdleAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Audio Components")
	TObjectPtr<UAudioComponent> AccelerateAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Audio Components")
	TObjectPtr<UAudioComponent> EngineAudioComponent;

	//Audio Variable
	UPROPERTY(Replicated, EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> IdleSound;

	UPROPERTY(Replicated, EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> AccelerateSound;

	UPROPERTY(Replicated, EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> EngineSound;

	UPROPERTY(BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating{ false };

	//Audio Functions
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySounds();

	UFUNCTION(Server, Reliable)
	void Server_RequestPlaySounds();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateAllClients();

	void UpdateSound(float DeltaTime);
};
