// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinTrigger.generated.h"

class UBoxComponent;
class ARacingVehicle;
UCLASS()
class MULTIPLAYERRACINGCAR_API AWinTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWinTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	TObjectPtr<ARacingVehicle> WinnerVehicle;

	//Handling Winner
	UFUNCTION(Server, Reliable)
	void Server_HandleWinner(ARacingVehicle* Winner);
	UFUNCTION(NetMulticast, Reliable)
	void Net_Multicast_HandleWinner(ARacingVehicle* Winner);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
