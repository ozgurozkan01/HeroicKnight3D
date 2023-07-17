 // Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

 class UGameSaveManager;
class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class AItem;
class UAnimMontage;
class USoundCue;
class AEnemy;
class AMainPlayerController;
class AWeaponStorage;

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dead	UMETA(DisplayName = "Dead"),

	EMS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal				UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum		UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted			UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecoviring UMETA(DisplayName = "ExhaustedRecoviring"),

	ESS_MAX					UMETA(DisplayName = "DefaultMAX")
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
	void ESCDown();
	void ESCUp();
	void SetMovementStatus(EMovementStatus CurrentStatus);
	void SetStaminaLevel();
	void Attack();
	void SetInterpToEnemy(bool bInterpTo);
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	void InterpRotationToTarget(float& DeltaTime);
	FRotator GetInterpRotationYaw(FVector TargetLocation);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	virtual void Jump() override;
	bool IsAlive();
	void Die();
	void UpdateCombatTarget();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();
	UFUNCTION(BlueprintCallable)
	void IncrementCoin(int32 TakenCoin);
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float TakenPotion);

	
	FORCEINLINE USpringArmComponent* GetSpringArm() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetCamera() const {return FollowCamera;}
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon;}
	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }
	FORCEINLINE void SetStaminaStatus(EStaminaStatus CurrentStatus) { StaminaStatus = CurrentStatus; }
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }
	FORCEINLINE void SetHasCombatTarget(bool HasCombatTarget) { bHasCombatTarget = HasCombatTarget; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	AEnemy* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Items")
	AItem* ActiveOverlappingItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attack")
	UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | Sound")
	USoundCue* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShiftKeyDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLMBDown;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bESCDown;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	bool bHasCombatTarget;

	bool bInterpToEnemy;
	bool bMoveForward;
	bool bMoveRight;
	
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float MinSprintStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float StaminaDrainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	FVector CombatTargetLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
	TSubclassOf<AEnemy> EnemyFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveGame")
	UGameSaveManager* GameSaveManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SaveGame")
	TSubclassOf<AWeaponStorage> WeaponStorage;
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	float MouseSensitivity;

	bool bIsInAir;
};