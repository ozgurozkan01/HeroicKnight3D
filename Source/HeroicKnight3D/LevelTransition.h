// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransition.generated.h"

class UBoxComponent;

UCLASS()
class HEROICKNIGHT3D_API ALevelTransition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransition();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	UBoxComponent* LevelTransitionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	UBillboardComponent* BillboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	FName TransitionLevelName;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void SwitchLevel(FName LevelName);
};
