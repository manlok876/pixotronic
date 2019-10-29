// Copyright 2019 Denis Maximenko

using UnrealBuildTool;
using System.Collections.Generic;

public class PixotronicEditorTarget : TargetRules
{
	public PixotronicEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Pixotronic");
	}
}
