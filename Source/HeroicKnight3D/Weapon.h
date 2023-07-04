// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class AMain;

UCLASS()
class HEROICKNIGHT3D_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	
	AWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	USkeletalMeshComponent* WeaponMesh;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void WeaponAttach(AMain* MainPlayer);
};
