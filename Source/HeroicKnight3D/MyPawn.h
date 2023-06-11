// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UPawnMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UPlayerMovementComponent;

UCLASS()
class HEROICKNIGHT3D_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	UPlayerMovementComponent* OurMovementComponent;
	
	void MoveForward(float inputValue);
	void MoveRight(float inputValue);
	void LookUp(float inputValue);
	void LookRight(float inputValue);
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	FVector2D CameraRotation;	
};
