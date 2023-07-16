// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStorage.generated.h"

class AWeapon;

UCLASS()
class HEROICKNIGHT3D_API AWeaponStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SaveData")
	TMap<FString, TSubclassOf<AWeapon>> WeaponMap;
};
