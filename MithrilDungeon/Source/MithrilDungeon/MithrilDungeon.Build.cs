// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MithrilDungeon : ModuleRules
{
	public MithrilDungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "PhysicsCore", "UMG", "Slate" , "SlateCore" });
	}
}
