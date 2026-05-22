// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiplayerRacingCar : ModuleRules
{
	public MultiplayerRacingCar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDependencyModuleNames.AddRange(new string[] {
        //	"Core",
        //	"CoreUObject",
        //	"Engine",
        //	"InputCore",
        //	"EnhancedInput",
        //	"AIModule",
        //	"StateTreeModule",
        //	"GameplayStateTreeModule",
        //	"UMG",
        //	"Slate", 
        //	"OnlineSubsystem", 
        //	"OnlineSubsystemSteam"
        //      });
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "UMG",
            "Slate",
            "SlateCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "SteamSockets"
        });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        PrivateDependencyModuleNames.AddRange(new string[] { "ChaosVehicles"});

		PublicIncludePaths.AddRange(new string[] {
			"MultiplayerRacingCar",
			"MultiplayerRacingCar/Variant_Platforming",
			"MultiplayerRacingCar/Variant_Platforming/Animation",
			"MultiplayerRacingCar/Variant_Combat",
			"MultiplayerRacingCar/Variant_Combat/AI",
			"MultiplayerRacingCar/Variant_Combat/Animation",
			"MultiplayerRacingCar/Variant_Combat/Gameplay",
			"MultiplayerRacingCar/Variant_Combat/Interfaces",
			"MultiplayerRacingCar/Variant_Combat/UI",
			"MultiplayerRacingCar/Variant_SideScrolling",
			"MultiplayerRacingCar/Variant_SideScrolling/AI",
			"MultiplayerRacingCar/Variant_SideScrolling/Gameplay",
			"MultiplayerRacingCar/Variant_SideScrolling/Interfaces",
			"MultiplayerRacingCar/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
