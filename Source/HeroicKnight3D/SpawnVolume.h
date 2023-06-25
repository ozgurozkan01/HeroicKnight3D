// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class AMyPawn;
class UBoxComponent;

UCLASS()
class HEROICKNIGHT3D_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AMyPawn> PawnToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	UParticleSystem* SpawningParticleEffect;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Spawning")
	void SpawnOurPawn(UClass* SpawnedPawn, const FVector& Location);
	
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void PawnSpawnedEffect(const FVector& Location);
};