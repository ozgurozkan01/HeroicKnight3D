// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class AMain;
class AController;

UENUM(BlueprintType)
enum class EWeaponState
{
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_PickUp UMETA(DisplayName = "PickUp"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class HEROICKNIGHT3D_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponState WeaponState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Items | Particles")
	bool bWeaponParticles;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | Sound")
	USoundCue* WeaponSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	UBoxComponent* CombatCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | Sound")
	USoundCue* SwingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TSubclassOf<UDamageType> DamageType;

	// Weapon is an actor. So it does not have any controller
	// We are gonna use main player controller.
	AController* WeaponInstegator;
	
protected:

	virtual void BeginPlay() override;
	
public:

	FORCEINLINE void SetWeaponState(EWeaponState CurrentState) { WeaponState = CurrentState; }
	FORCEINLINE void SetInstegator(AController* Instegator) { WeaponInstegator = Instegator; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
	void ActivateCombatCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateCombatCollision();
	void WeaponAttach(AMain* MainPlayer);
	
};
