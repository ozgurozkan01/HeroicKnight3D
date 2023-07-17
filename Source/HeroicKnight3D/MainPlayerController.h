// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HEROICKNIGHT3D_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/* Getting convenient class to create widget asset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> HUDOverlayAsset;

	/* Holder to widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* HUDOverlay;

	/* Getting convenient class to create widget asset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	/* Holder to widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	/* Holder to widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets")
	UUserWidget* PauseMenu;
	
	FVector EnemyLocation;
	
	void SetEnemyHealthBarProperties();
	
	// Show and remove the HUD on the player screen.
	bool bPauseMenuVisible; // Check HUD should be shown on the screen
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Widgets")
	void DisplayPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Widgets")
	void RemovePauseMenu();
	void TogglePauseMenu();
	
	bool bEnemyHealthBarVisible; // Check HUD should be shown on the screen
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();
protected: 

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
