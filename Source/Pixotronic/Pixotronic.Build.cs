// Copyright 2019 Denis Maximenko

using UnrealBuildTool;

public class Pixotronic : ModuleRules
{
	public Pixotronic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" });

         DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	}
}
