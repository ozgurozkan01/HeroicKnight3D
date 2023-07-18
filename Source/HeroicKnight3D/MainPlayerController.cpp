// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "GameSaveManager.h"
#include "Main.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::CreateSaveGameObject(UGameSaveManager::StaticClass()));
	MainPlayer = Cast<AMain>(UGameplayStatics::GetPlayerPawn(this, 0));
	
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

	PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

	if(PauseMenu)
	{
		PauseMenu->AddToViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);

		SaveGameButton = Cast<UButton>(PauseMenu->GetWidgetFromName(FName("SaveGameButton")));
		QuitGameButton = Cast<UButton>(PauseMenu->GetWidgetFromName(FName("QuitGameButton")));
		LoadGameButton = Cast<UButton>(PauseMenu->GetWidgetFromName(FName("LoadGameButton")));
		ResumeGameButton = Cast<UButton>(PauseMenu->GetWidgetFromName(FName("ResumeButton")));

		if (SaveGameButton && QuitGameButton && LoadGameButton && ResumeGameButton)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully"));
			SaveGameButton->OnClicked.AddDynamic(this, &AMainPlayerController::SaveGame);
			LoadGameButton->OnClicked.AddDynamic(this, &AMainPlayerController::LoadGame);
			QuitGameButton->OnClicked.AddDynamic(this, &AMainPlayerController::QuitGame);
			ResumeGameButton->OnClicked.AddDynamic(this, &AMainPlayerController::ResumeGame);
		}
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

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI; // Nothing but Game and UIs respond to inputs. They are not affected by inputs.
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = false;

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
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

void AMainPlayerController::SaveGame()
{
	if (GameSaveManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved!"));
		GameSaveManager->SaveGame(MainPlayer);
	}
}

void AMainPlayerController::LoadGame()
{
	if (GameSaveManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded!"));
		GameSaveManager->LoadGame(MainPlayer, true);
	}
}

void AMainPlayerController::ResumeGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Resume Game!!!"));
	RemovePauseMenu();
}

void AMainPlayerController::QuitGame()
{
	ConsoleCommand("quit");
}
