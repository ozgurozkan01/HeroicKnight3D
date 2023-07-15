// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "AIController.h"
#include "Enemy.h"
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

	
	
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
	
	/*
	FVector SpawnPoint = GetSpawnPoint();
	TSubclassOf<AActor> SpawnActor = GetSpawnActor();
	
	SpawnOurActor(SpawnActor, SpawnPoint);*/
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

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() == 0) { return nullptr;}

	int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);

	return SpawnArray[Selection];
}

void ASpawnVolume::PawnSpawnedEffect(const FVector& Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawningParticleEffect, Location, FRotator::ZeroRotator);
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* SpawnedPawn, const FVector& Location)
{
	UWorld* GameWorld = GetWorld();

	if(SpawnedPawn == nullptr || GameWorld == nullptr) return;
	
	AActor* Actor = GameWorld->SpawnActor<AActor>(SpawnedPawn, Location, FRotator::ZeroRotator);

	if (Actor)
	{
		AEnemy* SpawnedEnemy = Cast<AEnemy>(Actor);

		if (SpawnedEnemy)
		{
			SpawnedEnemy->SpawnDefaultController();
			AAIController* AIController = Cast<AAIController>(SpawnedEnemy->GetController());

			if (AIController)
			{
				SpawnedEnemy->AIController = AIController;
			}
		}
	}
}