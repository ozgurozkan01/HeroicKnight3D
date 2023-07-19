// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveManager.generated.h"

class AWeaponStorage;
class ALevelTransition;

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	float Health;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	uint32 Coins; 

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FVector Location;
	
	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FString LevelName;
};

UCLASS()
class HEROICKNIGHT3D_API UGameSaveManager : public USaveGame
{
	GENERATED_BODY()

public:
	UGameSaveManager();

	UPROPERTY(EditAnywhere, Category="SaveGame")
	FString PlayerName;

	UPROPERTY(EditAnywhere, Category="SaveGame")
	uint32 PlayerIndex;

	UPROPERTY(EditAnywhere, Category="SaveGame")
	FCharacterStats CharacterStats;
	
	UPROPERTY(EditAnywhere, Category="SaveGame")
	TSubclassOf<ALevelTransition> LevelTransition;
	
	UFUNCTION(BlueprintCallable)
	void SaveGame(AMain* MainPlayer);
	UFUNCTION(BlueprintCallable)
	void LoadGame(AMain* MainPlayer, bool bSetTransform);
	UFUNCTION(BlueprintCallable)
	void LoadGameNoSwitch(AMain* MainPlayer);

	
};
