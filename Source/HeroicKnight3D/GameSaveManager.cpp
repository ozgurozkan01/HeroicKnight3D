// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveManager.h"

#include "Main.h"
#include "Kismet/GameplayStatics.h"

UGameSaveManager::UGameSaveManager()
{
	PlayerName = TEXT("Knight");
	PlayerIndex = 0;
}

void UGameSaveManager::SaveGame(AMain* MainPlayer)
{
	CharacterStats.Health = MainPlayer->CurrentHealth;
	CharacterStats.MaxHealth = MainPlayer->MaxHealth;
	CharacterStats.Stamina = MainPlayer->CurrentStamina;
	CharacterStats.MaxStamina = MainPlayer->MaxStamina;
	CharacterStats.Coins = MainPlayer->Coin;

	CharacterStats.Location = MainPlayer->GetActorLocation();
	CharacterStats.Rotation = MainPlayer->GetActorRotation();

	UGameplayStatics::SaveGameToSlot(this, PlayerName, PlayerIndex);
}

void UGameSaveManager::LoadGame(AMain* MainPlayer, bool bSetTransform)
{

	UGameplayStatics::LoadGameFromSlot(PlayerName, PlayerIndex);

	MainPlayer->CurrentHealth = CharacterStats.Health;
	MainPlayer->MaxHealth = CharacterStats.MaxHealth;
	MainPlayer->CurrentStamina = CharacterStats.Stamina;
	MainPlayer->MaxStamina = CharacterStats.MaxStamina;
	MainPlayer->Coin = CharacterStats.Coins;

	if (bSetTransform)
	{
		MainPlayer->SetActorLocation(CharacterStats.Location);
		MainPlayer->SetActorRotation(CharacterStats.Rotation);
	}
}
