// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NoEndHouse : ModuleRules
{
	public NoEndHouse(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MoviePlayer", "RHI", 
            "UMG", "Slate", "SlateCore", "SteamVR", "SteamVRController", "HeadMountedDisplay"});
	}
}
