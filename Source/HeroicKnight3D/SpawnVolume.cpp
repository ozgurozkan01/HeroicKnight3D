// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Volume"));
	
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	SpawnOurPawn(PawnToSpawn, GetSpawnPoint());
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector VolumeExtent = SpawnVolume->GetScaledBoxExtent();
	FVector VolumeOrigin = SpawnVolume->GetComponentLocation();

	/* This function is returning random point which is in SpawnVolume that we created */
	FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(VolumeOrigin, VolumeExtent);

	return SpawnPoint;
}

void ASpawnVolume::PawnSpawnedEffect(const FVector& Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawningParticleEffect, Location, FRotator::ZeroRotator);
}

void ASpawnVolume::SpawnOurPawn_Implementation(UClass* SpawnedPawn, const FVector& Location)
{
	UWorld* GameWorld = GetWorld();

	if(SpawnedPawn == nullptr || GameWorld == nullptr) return;
	
	GameWorld->SpawnActor<AMyPawn>(PawnToSpawn, Location, FRotator::ZeroRotator);
}