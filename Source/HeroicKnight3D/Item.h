// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USoundCue;
class USphereComponent;

UCLASS()
class HEROICKNIGHT3D_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item | Collision")
	USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item | Mesh")
	UStaticMeshComponent* ItemMesh;

	/* Components needs to be constructed with CreateDefaultSubObject */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item | Particles")
	UParticleSystemComponent* IdleParticle;

	/* If class is not component, then it does not need to be constructed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Particles")
	UParticleSystem* OverlapParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item | Sounds")
	USoundCue* OverlapSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
