// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

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
	TSubclassOf<AActor> Actor_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AActor> Actor_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AActor> Actor_3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AActor> Actor_4;

	TArray<TSubclassOf<AActor>> SpawnArray;
	
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

	UFUNCTION(BlueprintPure, Category="Spawning")
	TSubclassOf<AActor> GetSpawnActor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Spawning")
	void SpawnOurActor(UClass* SpawnedPawn, const FVector& Location);
	
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void PawnSpawnedEffect(const FVector& Location);
};