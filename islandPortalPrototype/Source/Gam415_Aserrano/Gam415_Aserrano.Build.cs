// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gam415_Aserrano : ModuleRules
{
	public Gam415_Aserrano(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "ProceduralMeshComponent"});
	}
}
