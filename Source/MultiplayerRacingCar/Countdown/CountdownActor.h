// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CountdownActor.generated.h"

UCLASS()
class MULTIPLAYERRACINGCAR_API ACountdownActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountdownActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//Components
	UPROPERTY(VisibleAnywhere, Category = "CountdownActor")
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "CountdownActor", meta = (AllowPrivateAccess = "true"))
	int32 CountdownTime;

	UFUNCTION()
	void UpdateCountdown();

	UFUNCTION(Server, Reliable)
	void Server_StartCountdown();

	FTimerHandle CountdownTimerHandle;
};
