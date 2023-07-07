// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class AItem;
class UAnimMontage;

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX") 
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecoviring UMETA(DisplayName = "ExhaustedRecoviring"),
	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class HEROICKNIGHT3D_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void TurnAtRate(float Rate);
	void LookUpRate(float Rate);
	void ShiftKeyDown();
	void ShiftKeyUp();
	void LMBDown();
	void LMBUp();

	
	void SetMovementStatus(EMovementStatus CurrentStatus);
	void SetStaminaLevel();
	
	void DecrementHealth(float TakenDamage);
	void IncrementCoin(int32 TakenCoin);
	void Attack();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	void Die();
	
	FORCEINLINE USpringArmComponent* GetSpringArm() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetCamera() const {return FollowCamera;}
	FORCEINLINE void SetStaminaStatus(EStaminaStatus CurrentStatus) { StaminaStatus = CurrentStatus; }
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon;}
	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Coin")
	int32 Coin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats")
	EStaminaStatus StaminaStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Items")
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Items")
	AItem* ActiveOverlappingItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attack")
	UAnimMontage* CombatMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShiftKeyDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLMBDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	bool bAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float MinSprintStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float StaminaDrainRate;

private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	float MouseSensitivity;

	bool bIsInAir;
};
