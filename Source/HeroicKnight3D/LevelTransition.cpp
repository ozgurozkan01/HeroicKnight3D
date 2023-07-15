// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"

#include "Main.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	TransitionLevelName = "SunTample";
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
	if (OtherActor)
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if (MainPlayer)
		{
			SwitchLevel(TransitionLevelName);
		}
	}
}

void ALevelTransition::SwitchLevel(FName LevelName)
{
	UWorld* GameWorld = GetWorld();

	if (GameWorld)
	{
		FString CurrentLevelName = GameWorld->GetMapName();
	
		FName CurrentLevelFName(CurrentLevelName);
		if (CurrentLevelFName != LevelName)
		{
			UGameplayStatics::OpenLevel(GameWorld, LevelName);
		}
	}
}
