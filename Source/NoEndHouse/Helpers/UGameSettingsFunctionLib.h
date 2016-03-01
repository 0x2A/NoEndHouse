// Copyright (c) 2016 Stift15 Entertainment

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UGameSettingsFunctionLib.generated.h"

/**
* Screen Resolution
* @remark Engine type is not meant for blueprints so we replicate the type.
* @remark The Engine type uses uint32 but we need to use int32. Otherwise it won't be possible to break/make this type.
*/
USTRUCT(BlueprintType)
struct FScreenResolutionRHIBP
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScreenResolution)
		int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScreenResolution)
		int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScreenResolution)
		int32 RefreshRate;

	FScreenResolutionRHIBP()
	{
		Width = Height = RefreshRate = 0;
	}
};

USTRUCT(BlueprintType)
struct FGraphicSettings
{
	GENERATED_USTRUCT_BODY()

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		FScreenResolutionRHIBP Resolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 WindowMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		bool VSync;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 ViewDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 AntiAliasing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 ShadowQuality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 TextureResolution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 EffectsQuality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		int32 PostProcessing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GraphicSettings)
		bool Valid;
};

UCLASS()
class NOENDHOUSE_API UGameSettingsFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Retrieve a sorted list of all screen resolutions supported by the player's display adapter.
	*
	* @returns the array of all supported screen resolutions.
	*/
	UFUNCTION(BlueprintPure, Category = "GameSettings|Graphics")
	static TArray<struct FScreenResolutionRHIBP> GetDisplayAdapterScreenResolutions(bool ignoreRefreshRate = false);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Graphics")
		static struct FGraphicSettings GetUserGraphicSettings();

	UFUNCTION(BlueprintCallable, Category = "GameSettings|Graphics")
		static void ApplyUserGraphicSettings(struct FGraphicSettings settings);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Audio")
		static int32 GetUserAudioQuality();

	UFUNCTION(BlueprintCallable, Category = "GameSettings|Audio")
		static void ApplyUserAudioQuality(int32 quality);

	UFUNCTION(BlueprintCallable, Category = "GameSettings|Audio")
		static void SetSoundVolume(USoundClass* soundClassObject, float newVolume);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Audio")
		static float GetSoundVolume(USoundClass* soundClassObject);
};