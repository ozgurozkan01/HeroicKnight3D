// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class HEROICKNIGHT3D_API APickUp : public AItem
{
	GENERATED_BODY()

public:

	APickUp();

	int32 PickedCoin;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// We created a impelementable event function,
	// Cause we use this event more than one BP class.
	UFUNCTION(BlueprintImplementableEvent)
	void OnPickUpBP(AMain* MainPlayer);
};
