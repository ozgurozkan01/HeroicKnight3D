#include "GameSaveManager.h"

#include "LevelTransition.h"
#include "Main.h"
#include "Weapon.h"
#include "WeaponStorage.h"
#include "Kismet/GameplayStatics.h"

UGameSaveManager::UGameSaveManager()
{
	PlayerName = TEXT("Knight");
	PlayerIndex = 0;
	CharacterStats.WeaponName = TEXT("");
	CharacterStats.LevelName = TEXT("");
}

void UGameSaveManager::SaveGame(AMain* MainPlayer)
{
	UGameSaveManager* GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::CreateSaveGameObject(UGameSaveManager::StaticClass()));
	
	GameSaveManager->CharacterStats.Health = MainPlayer->CurrentHealth;
	GameSaveManager->CharacterStats.MaxHealth = MainPlayer->MaxHealth;
	GameSaveManager->CharacterStats.Stamina = MainPlayer->CurrentStamina;
	GameSaveManager->CharacterStats.MaxStamina = MainPlayer->MaxStamina;
	GameSaveManager->CharacterStats.Coins = MainPlayer->Coin;
	GameSaveManager->CharacterStats.FirstAttackSpeed = MainPlayer->FirstAttackAnimationRate;
	GameSaveManager->CharacterStats.SecondAttackSpeed = MainPlayer->SecondAttackAnimationRate;
	GameSaveManager->CharacterStats.KillAmount = MainPlayer->KillAmount;
	
	
	GameSaveManager->CharacterStats.Location = MainPlayer->GetActorLocation();
	GameSaveManager->CharacterStats.Rotation = MainPlayer->GetActorRotation();
	
	FString MapName = MainPlayer->GetWorld()->GetMapName();
	MapName.RemoveFromStart(MainPlayer->GetWorld()->StreamingLevelsPrefix);

	GameSaveManager->CharacterStats.LevelName = MapName;
	
	if (MainPlayer->EquippedWeapon)
	{
		FString EquippedWeaponName = MainPlayer->EquippedWeapon->WeaponName; 
		GameSaveManager->CharacterStats.WeaponName = EquippedWeaponName;
	}
	
	UGameplayStatics::SaveGameToSlot(GameSaveManager, PlayerName, PlayerIndex);
}

void UGameSaveManager::LoadGame(AMain* MainPlayer, bool bSetTransform)
{
	UGameSaveManager* GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::CreateSaveGameObject(UGameSaveManager::StaticClass()));
	GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::LoadGameFromSlot(PlayerName, PlayerIndex));

	MainPlayer->CurrentHealth = GameSaveManager->CharacterStats.Health;
	MainPlayer->MaxHealth = GameSaveManager->CharacterStats.MaxHealth;
	MainPlayer->CurrentStamina = GameSaveManager->CharacterStats.Stamina;
	MainPlayer->MaxStamina = GameSaveManager->CharacterStats.MaxStamina;
	MainPlayer->Coin = GameSaveManager->CharacterStats.Coins;
	MainPlayer->FirstAttackAnimationRate = GameSaveManager->CharacterStats.FirstAttackSpeed;
	MainPlayer->SecondAttackAnimationRate = GameSaveManager->CharacterStats.SecondAttackSpeed;
	MainPlayer->KillAmount = GameSaveManager->CharacterStats.KillAmount;
	
	if (MainPlayer->WeaponStorage)
	{
		AWeaponStorage* Weapons = MainPlayer->GetWorld()->SpawnActor<AWeaponStorage>(MainPlayer->WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = GameSaveManager->CharacterStats.WeaponName;

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
		MainPlayer->SetActorLocation(GameSaveManager->CharacterStats.Location);
		MainPlayer->SetActorRotation(GameSaveManager->CharacterStats.Rotation);
	}

	MainPlayer->SetMovementStatus(EMovementStatus::EMS_Normal);
	MainPlayer->GetMesh()->bPauseAnims = false;
	MainPlayer->GetMesh()->bNoSkeletonUpdate = false;

	if (GameSaveManager->CharacterStats.LevelName != TEXT(""))
	{

		ALevelTransition* LevelTransition = Cast<ALevelTransition>(MainPlayer->GetWorld()->SpawnActor<ALevelTransition>());
		if (LevelTransition)
		{
			FName LevelName(GameSaveManager->CharacterStats.LevelName);
			LevelTransition->SwitchLevel(LevelName);
		}
	}
}

void UGameSaveManager::LoadGameNoSwitch(AMain* MainPlayer)
{
	UGameSaveManager* GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::CreateSaveGameObject(UGameSaveManager::StaticClass()));
	GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::LoadGameFromSlot(PlayerName, PlayerIndex));

	MainPlayer->CurrentHealth = GameSaveManager->CharacterStats.Health;
	MainPlayer->MaxHealth = GameSaveManager->CharacterStats.MaxHealth;
	MainPlayer->CurrentStamina = GameSaveManager->CharacterStats.Stamina;
	MainPlayer->MaxStamina = GameSaveManager->CharacterStats.MaxStamina;
	MainPlayer->Coin = GameSaveManager->CharacterStats.Coins;
	MainPlayer->FirstAttackAnimationRate = GameSaveManager->CharacterStats.FirstAttackSpeed;
	MainPlayer->SecondAttackAnimationRate = GameSaveManager->CharacterStats.SecondAttackSpeed;
	MainPlayer->KillAmount = GameSaveManager->CharacterStats.KillAmount;
	
	if (MainPlayer->WeaponStorage)
	{
		AWeaponStorage* Weapons = MainPlayer->GetWorld()->SpawnActor<AWeaponStorage>(MainPlayer->WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = GameSaveManager->CharacterStats.WeaponName;

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

	MainPlayer->SetMovementStatus(EMovementStatus::EMS_Normal);
	MainPlayer->GetMesh()->bPauseAnims = false;
	MainPlayer->GetMesh()->bNoSkeletonUpdate = false;
}