// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class HEROICKNIGHT3D_API UPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
