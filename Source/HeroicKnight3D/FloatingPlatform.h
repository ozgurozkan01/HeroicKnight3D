// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class HEROICKNIGHT3D_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Floating Platform")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Floating Platform")
	FVector StartPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MakeEditWidget = "true"), Category="Floating Platform")
	FVector EndPoint;

	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	UPROPERTY(EditAnywhere)
	bool bInterping;
	
	FTimerHandle InterpTimerHandle;

	float InterpTime;
	
	float Distance;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();
	void SwapVectors(FVector& Start, FVector& End);
};
