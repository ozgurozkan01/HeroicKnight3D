// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f)); // Sizes of box

	/*	Hard Codding - Making in C++ what we can make in blueprint easily !!!
	 *TriggerBox->SetGenerateOverlapEvents(true);
	 *TriggerBox->SetLineThickness(5.f);
	*/
	
	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	SwitchDelayTime = 3.f;
	bCharacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation(); // Door is component of Trigger Box
	InitialSwitchLocation = FloorSwitch->GetComponentLocation(); // Floor Switch is a component of Trigger Box
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// When it is pressed on the switch and it lowers, at the same time door raises.
	if (!bCharacterOnSwitch) bCharacterOnSwitch = true;

	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (bCharacterOnSwitch) bCharacterOnSwitch = false;
	
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchDelayTime);
}

void AFloorSwitch::UpdateDoorLocation(float VectorZ)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += VectorZ;
	Door->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchLocation(float VectorZ)
{
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += VectorZ;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	// When it is stoppped pressing on the switch and it raises, at the same time door lowers.
	if (!bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();	
	}
}
