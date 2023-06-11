// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f); // It makes a vector normalize 

	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), DesiredMovementThisFrame.X, DesiredMovementThisFrame.Y, DesiredMovementThisFrame.Z);
	
	if(!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true ,Hit);

		if (Hit.IsValidBlockingHit())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Something"));
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
