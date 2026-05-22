// Fill out your copyright notice in the Description page of Project Settings.


#include "WinTrigger.h"
#include "Components/BoxComponent.h"
#include "MultiplayerRacingCar/Vehicle/RacingVehicle.h"
#include "EngineUtils.h"
#include "UI/GameEndAnnouncement.h"

// Sets default values
AWinTrigger::AWinTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AWinTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWinTrigger::OnOverlapBegin);
}

// Called every frame
void AWinTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWinTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARacingVehicle* Vehicle = Cast<ARacingVehicle>(OtherActor))
	{
		if (WinnerVehicle == nullptr) {
			if (HasAuthority()) {
				Net_Multicast_HandleWinner(Vehicle);
			}
			else {
				Server_HandleWinner(Vehicle);
			}
		}
	}
}

void AWinTrigger::Server_HandleWinner_Implementation(ARacingVehicle* Winner)
{
	if (HasAuthority() && WinnerVehicle == nullptr)
	{
		WinnerVehicle = Winner;
		Net_Multicast_HandleWinner(WinnerVehicle);
	}
}

void AWinTrigger::Net_Multicast_HandleWinner_Implementation(ARacingVehicle* Winner)
{
	for (ARacingVehicle* PlayerVehicle : TActorRange<ARacingVehicle>(GetWorld())) {
		if (PlayerVehicle && PlayerVehicle->IsLocallyControlled())
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(PlayerVehicle->GetController()))
			{
				PlayerVehicle->DisableInput(PlayerController);
			}
			if (PlayerVehicle == Winner)
			{
				PlayerVehicle->GameAnnouncement->DisplayEndGameMenu(FString("YOU WON!"));
			}
			else {
				PlayerVehicle->GameAnnouncement->DisplayEndGameMenu(FString("YOU LOST!"));
			}
		}
	}
}
