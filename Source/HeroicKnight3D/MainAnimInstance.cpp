// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "Main.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			MainPlayer = Cast<AMain>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
		
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (MainPlayer == nullptr)
		{
			MainPlayer = Cast<AMain>(Pawn);
		}
	}
}