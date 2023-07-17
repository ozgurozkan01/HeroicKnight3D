// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveManager.h"

#include "Main.h"
#include "Weapon.h"
#include "WeaponStorage.h"
#include "Kismet/GameplayStatics.h"

UGameSaveManager::UGameSaveManager()
{
	PlayerName = TEXT("Knight");
	PlayerIndex = 0;
	CharacterStats.WeaponName = TEXT("");
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

	if (MainPlayer->EquippedWeapon)
	{
		FString EquippedWeaponName = MainPlayer->EquippedWeapon->WeaponName; 
		CharacterStats.WeaponName = EquippedWeaponName;
	}
	
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

	UE_LOG(LogTemp, Warning, TEXT("%f"), CharacterStats.Health);	
	UE_LOG(LogTemp, Warning, TEXT("%f"), CharacterStats.MaxHealth);	
	UE_LOG(LogTemp, Warning, TEXT("%f"), CharacterStats.Stamina);	
	UE_LOG(LogTemp, Warning, TEXT("%f"), CharacterStats.MaxStamina);	

	
	if (MainPlayer->WeaponStorage)
	{
		AWeaponStorage* Weapons = MainPlayer->GetWorld()->SpawnActor<AWeaponStorage>(MainPlayer->WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = CharacterStats.WeaponName;

			if (Weapons->WeaponMap.Contains(WeaponName))
			{
				AWeapon* EquippedWeapon = MainPlayer->GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);

				if (EquippedWeapon)
				{
					EquippedWeapon->WeaponAttach(MainPlayer);
				}
			}
		}
	}
	
	if (bSetTransform)
	{
		MainPlayer->SetActorLocation(CharacterStats.Location);
		MainPlayer->SetActorRotation(CharacterStats.Rotation);
	}
}
