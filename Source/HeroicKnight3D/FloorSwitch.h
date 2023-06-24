// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

class UBoxComponent;

UCLASS()
class HEROICKNIGHT3D_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floow Switch")
	UBoxComponent* TriggerBox; // It is created for Trigger Volume

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floow Switch")
	UStaticMeshComponent* FloorSwitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floow Switch")
	UStaticMeshComponent* Door;

	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	FVector InitialDoorLocation;

	UPROPERTY(BlueprintReadWrite, Category="Floor Switch")
	FVector InitialSwitchLocation;

	FTimerHandle SwitchHandle;

	float SwitchDelayTime;
	bool bCharacterOnSwitch;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION() // If we bind a function to the delegate It has to be UFUNCTION
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION() // If we bind a function to the delegate It has to be UFUNCTION
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Floor Switch") // It is used for appropriate the function
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category="Floor Switch") // It is used for appropriate the function
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category="Floor Switch") // It is used for appropriate the function
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category="Floor Switch") // It is used for appropriate the function
	void LowerFloorSwitch();

	/* Set door location in every frame */
	UFUNCTION(BlueprintCallable, Category="Floor Switch")
	void UpdateDoorLocation(float VectorZ);

	/* Set switch location in every frame */
	UFUNCTION(BlueprintCallable, Category="Floor Switch")
	void UpdateSwitchLocation(float VectorZ);

	void CloseDoor();
};