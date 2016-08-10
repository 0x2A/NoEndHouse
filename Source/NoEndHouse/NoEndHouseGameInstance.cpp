// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "NoEndHouseGameInstance.h"
#include "MoviePlayer.h"
#include "ContentStreaming.h"



void UNoEndHouseGameInstance::Init()
{
	UGameInstance::Init();

	bFirstLoaded = false;

	//Add delegates to hook up to the loading process
	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNoEndHouseGameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMap.AddUObject(this, &UNoEndHouseGameInstance::EndLoadingScreen);

}

void UNoEndHouseGameInstance::BeginLoadingScreen()
{
	if (!bFirstLoaded)
	{
		bFirstLoaded = true;
		return;
	}

	FLoadingScreenAttributes loadingScreen;
	loadingScreen.bAutoCompleteWhenLoadingCompletes = true;
	loadingScreen.MinimumLoadingScreenDisplayTime = 2;

	// workaround: since there is no looping functionality, we pass in the same loading
	// video multiple times so they are played one after another. 30 videos should be
	// a length of about 3.5 minutes
	for (int i = 0; i < 30; i++)
		loadingScreen.MoviePaths.Add(TEXT("LoadingScreen"));
	
	auto moviePlayer = GetMoviePlayer();
	moviePlayer->SetupLoadingScreen(loadingScreen);
	moviePlayer->PlayMovie();
}

void UNoEndHouseGameInstance::EndLoadingScreen()
{

}
