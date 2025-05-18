// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GardenSandbox : ModuleRules
{
	public GardenSandbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
