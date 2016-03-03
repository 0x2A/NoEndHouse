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
	auto userSettings = GEngine->GetGameUserSettings();
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

	auto userSettings = GEngine->GetGameUserSettings();
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
	GEngine->GetGameUserSettings()->SetAudioQualityLevel(quality);
	Scalability::SaveState(GGameUserSettingsIni);
	GEngine->GetGameUserSettings()->ApplySettings();
}

int32 UGameSettingsFunctionLib::GetUserAudioQuality()
{
	if (GEngine == nullptr) return 2;
	return GEngine->GetGameUserSettings()->GetAudioQualityLevel();
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

void UGameSettingsFunctionLib::GetAllActionKeyBindings(TArray<FInput>& Bindings)
{
	Bindings.Empty();

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return;

	const TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;
	for (const FInputActionKeyMapping& Each : Actions)
	{
		if (!Each.Key.IsGamepadKey())
			Bindings.Add(FInput(Each));
	}
}

FInput UGameSettingsFunctionLib::GetInputFromKeyEvent(const FKeyEvent& KeyEvent)
{
	FInput fInput;
	fInput.Key = KeyEvent.GetKey();
	fInput.KeyAsString = fInput.Key.GetDisplayName().ToString();

	fInput.bAlt = KeyEvent.IsAltDown();
	fInput.bCtrl = KeyEvent.IsControlDown();
	fInput.bShift = KeyEvent.IsShiftDown();
	fInput.bCmd = KeyEvent.IsCommandDown();

	return fInput;
}

bool UGameSettingsFunctionLib::ReBindActionKey(FInput Original, FInput NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;

	//~~~

	bool Found = false;
	for (FInputActionKeyMapping& Each : Actions)
	{
		//Search by original
		if (Each.ActionName.ToString() == Original.ActionName &&
			Each.Key == Original.Key
			){
			//Update to new!
			UpdateActionMapping(Each, NewBinding);
			Found = true;
			break;
		}
	}

	if (Found)
	{
		//SAVES TO DISK
		const_cast<UInputSettings*>(Settings)->SaveKeyMappings();

		//REBUILDS INPUT, creates modified config in Saved/Config/Windows/Input.ini
		for (TObjectIterator<UPlayerInput> It; It; ++It)
		{
			It->ForceRebuildingKeyMaps(true);
		}
	}
	return Found;
}

void UGameSettingsFunctionLib::GetAllAxisKeyBindings(TArray<FInputAxis>& Bindings)
{
	Bindings.Empty();

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return;

	const TArray<FInputAxisKeyMapping>& Axi = Settings->AxisMappings;

	for (const FInputAxisKeyMapping& Each : Axi)
	{
		if(!(Each.Key.IsGamepadKey() ||Each.Key.IsMouseButton()))
			Bindings.Add(FInputAxis(Each));
	}
}

FInputAxis UGameSettingsFunctionLib::GetInputAxisFromKeyEvent(const FKeyEvent& KeyEvent)
{
	FInputAxis VInput;

	VInput.Key = KeyEvent.GetKey();
	VInput.KeyAsString = VInput.Key.GetDisplayName().ToString();
	VInput.Scale = 1;

	return VInput;
}

bool UGameSettingsFunctionLib::ReBindAxisKey(FInputAxis Original, FInputAxis NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	TArray<FInputAxisKeyMapping>& Axi = Settings->AxisMappings;

	//~~~

	bool Found = false;
	for (FInputAxisKeyMapping& Each : Axi)
	{
		//Search by original
		if (Each.AxisName.ToString() == Original.AxisName &&
			Each.Key == Original.Key && Each.Scale == Original.Scale)
		{
			//Update to new!
			UpdateAxisMapping(Each, NewBinding);
			Found = true;
			break;
		}
	}

	if (Found)
	{
		//SAVES TO DISK
		const_cast<UInputSettings*>(Settings)->SaveKeyMappings();

		//REBUILDS INPUT, creates modified config in Saved/Config/Windows/Input.ini
		for (TObjectIterator<UPlayerInput> It; It; ++It)
		{
			It->ForceRebuildingKeyMaps(true);
		}
	}
	return Found;
}

TArray<FInputAxis> UGameSettingsFunctionLib::GetInputAxisFromAxisName(const FString& AxisName, float scale)
{
	TArray<FInputAxis> entries;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return entries;

	const TArray<FInputAxisKeyMapping>& Axi = Settings->AxisMappings;

	for (const FInputAxisKeyMapping& Each : Axi)
	{
		if (!Each.Key.IsGamepadKey() && Each.AxisName.ToString().Equals(AxisName, ESearchCase::IgnoreCase))
		{
			if (Each.Scale == scale)
				entries.Add(FInputAxis(Each));
		}
	}

	return entries;
}

TArray<FInput> UGameSettingsFunctionLib::GetInputsFromActionName(const FString& actionName)
{
	TArray<FInput> entries;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return entries;

	const TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;
	for (const FInputActionKeyMapping& Each : Actions)
	{
		if (!Each.Key.IsGamepadKey() && Each.ActionName.ToString().Equals(actionName, ESearchCase::IgnoreCase))
			entries.Add(FInput(Each));
	}
	return entries;
}

