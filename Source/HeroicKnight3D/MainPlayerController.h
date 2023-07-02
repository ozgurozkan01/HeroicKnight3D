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

protected:

	virtual void BeginPlay() override;
};
