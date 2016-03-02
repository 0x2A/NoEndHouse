// Copyright (c) 2016 Stift15 Entertainment

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerInput.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

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

USTRUCT(BlueprintType)
struct FInput
{
	GENERATED_USTRUCT_BODY()

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FString ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FString KeyAsString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		uint32 bShift : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		uint32 bCtrl : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		uint32 bAlt : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		uint32 bCmd : 1;


	FInput(){}
	FInput(const FString InActionName, const FKey InKey, const bool bInShift, const bool bInCtrl, const bool bInAlt, const bool bInCmd)
		: Key(InKey)
		, KeyAsString(Key.GetDisplayName().ToString())
		, bShift(bInShift)
		, bCtrl(bInCtrl)
		, bAlt(bInAlt)
		, bCmd(bInCmd)
	{
		ActionName = InActionName;
	}

	FInput(const FInputActionKeyMapping& Action)
		: Key(Action.Key)
		, KeyAsString(Action.Key.GetDisplayName().ToString())
		, bShift(Action.bShift)
		, bCtrl(Action.bCtrl)
		, bAlt(Action.bAlt)
		, bCmd(Action.bCmd)
	{
		ActionName = Action.ActionName.ToString();
	}
};

USTRUCT(BlueprintType)
struct FInputAxis
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FString AxisName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FString KeyAsString = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		FKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSettings|Input")
		float Scale = 1;

	FInputAxis(){}
	FInputAxis(const FString InAxisName, FKey InKey, float InScale)
		: Key(InKey)
		, KeyAsString(InKey.GetDisplayName().ToString())
		, AxisName(InAxisName)
		, Scale(InScale)
	{ }

	FInputAxis(const FInputAxisKeyMapping& Axis)
		: Key(Axis.Key)
		, KeyAsString(Axis.Key.GetDisplayName().ToString())
		, Scale(Axis.Scale)
	{
		AxisName = Axis.AxisName.ToString();
	}
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

	UFUNCTION(BlueprintPure, Category = "GameSettings|Input")
		static void GetAllActionKeyBindings(TArray<FInput>& Bindings);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Input")
		static void GetAllAxisKeyBindings(TArray<FInputAxis>& Bindings);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Input")
		static FInput GetInputFromKeyEvent(const FKeyEvent& KeyEvent);

	UFUNCTION(BlueprintPure, Category = "GameSettings|Input")
		static FInputAxis GetInputAxisFromKeyEvent(const FKeyEvent& KeyEvent);

	UFUNCTION(BlueprintCallable, Category = "GameSettings|Input")
		static bool ReBindActionKey(FInput Original, FInput NewBinding);

	UFUNCTION(BlueprintCallable, Category = "GameSettings|Input")
		static bool ReBindAxisKey(FInputAxis Original, FInputAxis NewBinding);

	static FORCEINLINE void UpdateActionMapping(FInputActionKeyMapping& Destination, const FInput& VictoryInputBind)
	{
		Destination.Key = VictoryInputBind.Key;
		Destination.bShift = VictoryInputBind.bShift;
		Destination.bCtrl = VictoryInputBind.bCtrl;
		Destination.bAlt = VictoryInputBind.bAlt;
		Destination.bCmd = VictoryInputBind.bCmd;
	}

	static FORCEINLINE void UpdateAxisMapping(FInputAxisKeyMapping& Destination, const FInputAxis& VictoryInputBind)
	{
		Destination.Key = VictoryInputBind.Key;
		Destination.Scale = VictoryInputBind.Scale;
	}
};