// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheBoat : ModuleRules
{
	public TheBoat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"Sockets",
				"Networking"
			}
		);
	}
}