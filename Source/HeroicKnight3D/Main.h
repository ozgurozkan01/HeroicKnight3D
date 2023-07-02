// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

class UCameraComponent;
class USpringArmComponent;

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
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"), Category="Camera")
	float MouseSensitivity;

	bool IsInAir;
};
