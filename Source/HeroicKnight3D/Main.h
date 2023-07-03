// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX") 
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

	void SetMovementStatus(EMovementStatus CurrentStatus);
	void DecrementHealth(float TakenDamage);
	void IncrementCoin(int32 TakenCoin);
	void Die();
	
	FORCEINLINE USpringArmComponent* GetSpringArm() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetCamera() const {return FollowCamera;}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	int32 Coin;

	EMovementStatus MovementStatus;
	
	bool bShiftKeyDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float RunningSpeed;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	float MouseSensitivity;

	bool IsInAir;
};
