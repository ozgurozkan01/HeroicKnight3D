// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LevelTransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelTransitionVolume"));
	SetRootComponent(LevelTransitionVolume);
	LevelTransitionVolume->SetBoxExtent(FVector(45.f, 45.f, 45.f));

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();

	LevelTransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnOverlapBegin);
}

void ALevelTransition::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}