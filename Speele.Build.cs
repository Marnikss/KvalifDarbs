// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Speele : ModuleRules
{
	public Speele(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore", 
			"EnhancedInput", 
			"SlateCore",
			"Slate", 
			"UMG",
			"AIModule",
			"NavigationSystem"
		});
	}
}
