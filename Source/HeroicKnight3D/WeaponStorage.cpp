// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponStorage.h"

// Sets default values
AWeaponStorage::AWeaponStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

