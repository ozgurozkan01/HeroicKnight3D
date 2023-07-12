// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset == nullptr) {return;}

	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);

	if(HUDOverlay)
	{
		HUDOverlay->AddToViewport(); // provide that widget covers whole screen 
		HUDOverlay->SetVisibility(ESlateVisibility::Visible); // make widget visible in the game 
	}
	
	if (WEnemyHealthBar == nullptr) {return;}
 
	EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);

	if(EnemyHealthBar)
	{
		EnemyHealthBar->AddToViewport();
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);

		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetEnemyHealthBarProperties();
}

void AMainPlayerController::SetEnemyHealthBarProperties()
{
	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		// Get 2D position on screen which is converted from 3D position in game
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 80.f;
		
		// We set the size of HUD. Otherwise, HUD is gonna cover whole game screen.
		FVector2D SizeInViewport(300.f, 25.f); 
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
