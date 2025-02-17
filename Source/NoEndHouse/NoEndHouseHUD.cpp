// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "NoEndHouse.h"
#include "NoEndHouseHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

ANoEndHouseHUD::ANoEndHouseHUD()
{
	bShowGrabIcon = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D> grabTexObj(TEXT("Texture2D'/Game/Textures/grabIcon.grabIcon'"));
	GrabTex = grabTexObj.Object;
}


void ANoEndHouseHUD::DrawHUD()
{
	Super::DrawHUD();


	if (bShowGrabIcon)
	{
		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D GrabIconDrawPosition((Center.X - (GrabTex->GetSurfaceWidth() * 0.5)),
			(Center.Y - (GrabTex->GetSurfaceHeight() * 0.5f)));

		FCanvasTileItem TileItem(GrabIconDrawPosition, GrabTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}

	// Draw very simple crosshair

	// find center of the Canvas
	/*const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
										   (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)) );

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );*/
}

void ANoEndHouseHUD::ShowGrabIcon()
{
	bShowGrabIcon = true;
}

void ANoEndHouseHUD::HideGrabIcon()
{
	bShowGrabIcon = false;
}

