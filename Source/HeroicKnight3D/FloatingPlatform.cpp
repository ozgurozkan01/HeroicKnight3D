// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	SetRootComponent(PlatformMesh);
	
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);
	InterpSpeed = 2.f;
	InterpTime = 1.f;
	
	bInterping = false;
	Distance = 0.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(InterpTimerHandle, this, &AFloatingPlatform::ToggleInterping, InterpTime);
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	Distance = (StartPoint - EndPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		SetActorLocation(Interp);

		float TraveledDistance = (GetActorLocation() - StartPoint).Size();

		if(Distance - TraveledDistance <= 0.1f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimerHandle, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& Start, FVector& End)
{
	FVector Temp = Start;
	Start = End;
	End = Temp;
}