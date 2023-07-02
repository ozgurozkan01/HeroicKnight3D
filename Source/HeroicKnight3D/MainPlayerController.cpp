// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset == nullptr) {return;}

	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	HUDOverlay->AddToViewport(); // provide that widget covers whole screen 
	HUDOverlay->SetVisibility(ESlateVisibility::Visible); // make widget visible in the game 
}
