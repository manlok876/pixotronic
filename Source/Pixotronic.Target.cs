// Copyright 2019 Denis Maximenko

using UnrealBuildTool;
using System.Collections.Generic;

public class PixotronicTarget : TargetRules
{
	public PixotronicTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Pixotronic");
	}
}
