// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "UGameSettingsFunctionLib.h"
#include "Engine.h"

TArray<FScreenResolutionRHIBP> UGameSettingsFunctionLib::GetDisplayAdapterScreenResolutions(bool ignoreRefreshRate)
{
	TArray<FScreenResolutionRHIBP> ResolutionsToReturn;
	FScreenResolutionArray Resolutions;
	if (RHIGetAvailableResolutions(Resolutions, ignoreRefreshRate))
	{
		// Preallocate just enough memory to store all elements
		ResolutionsToReturn.Reserve(Resolutions.Num());

		for (const FScreenResolutionRHI& EachResolution : Resolutions)
		{
			FScreenResolutionRHIBP resolution;
			resolution.Width = EachResolution.Width;
			resolution.Height = EachResolution.Height;
			resolution.RefreshRate = EachResolution.RefreshRate;

			ResolutionsToReturn.Add(resolution);
		}
	}

	return ResolutionsToReturn;
}

FGraphicSettings UGameSettingsFunctionLib::GetUserGraphicSettings()
{
	FGraphicSettings settings;
	if (GEngine == nullptr)
	{
		settings.Valid = false;
		return settings;
	}
	settings.Valid = true;
	auto userSettings = GEngine->GameUserSettings;
	auto resolution = userSettings->GetScreenResolution();
	settings.Resolution.Width = resolution.X;
	settings.Resolution.Height = resolution.Y;
	switch (userSettings->GetFullscreenMode())
	{
	case 1:
		settings.WindowMode = 1; break;
	case 2:
		settings.WindowMode = 0; break;
	default:
	case 0:
		settings.WindowMode = 2; break;
	}
	settings.VSync = userSettings->bUseVSync;
	settings.ViewDistance = userSettings->ScalabilityQuality.ViewDistanceQuality;
	settings.AntiAliasing = userSettings->ScalabilityQuality.AntiAliasingQuality;
	settings.ShadowQuality = userSettings->ScalabilityQuality.ShadowQuality - 1;
	settings.TextureResolution = userSettings->ScalabilityQuality.TextureQuality;
	settings.EffectsQuality = userSettings->ScalabilityQuality.EffectsQuality;
	settings.PostProcessing = userSettings->ScalabilityQuality.PostProcessQuality;
	return settings;
}

void UGameSettingsFunctionLib::ApplyUserGraphicSettings(FGraphicSettings settings)
{
	if (GEngine == nullptr) return;

	if (!settings.Valid) return;

	auto userSettings = GEngine->GameUserSettings;
	EWindowMode::Type mode;
	switch (settings.WindowMode)
	{
	case 1:
		mode = EWindowMode::WindowedFullscreen; break;
	case 2:
		mode = EWindowMode::Fullscreen; break;
	default:
	case 0:
		mode = EWindowMode::Windowed; break;
	}
	userSettings->SetVSyncEnabled(settings.VSync);
	userSettings->SetFullscreenMode(mode);
	userSettings->ScalabilityQuality.ViewDistanceQuality = settings.ViewDistance;
	userSettings->ScalabilityQuality.AntiAliasingQuality = settings.AntiAliasing;
	userSettings->ScalabilityQuality.ShadowQuality = settings.ShadowQuality + 1;
	userSettings->ScalabilityQuality.TextureQuality = settings.TextureResolution;
	userSettings->ScalabilityQuality.EffectsQuality = settings.EffectsQuality;
	userSettings->ScalabilityQuality.PostProcessQuality = settings.PostProcessing;
	userSettings->ConfirmVideoMode();
	userSettings->RequestResolutionChange(settings.Resolution.Width, settings.Resolution.Height, mode, true);
	userSettings->SetScreenResolution(FIntPoint(settings.Resolution.Width, settings.Resolution.Height));
	Scalability::SaveState(GGameUserSettingsIni);
	userSettings->ApplySettings();
}

void UGameSettingsFunctionLib::ApplyUserAudioQuality(int32 quality)
{
	if (GEngine == nullptr) return;
	GEngine->GameUserSettings->SetAudioQualityLevel(quality);
	Scalability::SaveState(GGameUserSettingsIni);
	GEngine->GameUserSettings->ApplySettings();
}

int32 UGameSettingsFunctionLib::GetUserAudioQuality()
{
	if (GEngine == nullptr) return 2;
	return GEngine->GameUserSettings->GetAudioQualityLevel();
}

float UGameSettingsFunctionLib::GetSoundVolume(USoundClass* soundClassObject)
{
	if (!soundClassObject) return -1;

	return soundClassObject->Properties.Volume;
}

void UGameSettingsFunctionLib::SetSoundVolume(USoundClass* soundClassObject, float newVolume)
{
	if (!soundClassObject) return;
	soundClassObject->Properties.Volume = newVolume;
}

