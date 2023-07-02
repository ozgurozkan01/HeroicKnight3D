// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Volume"));
	SetRootComponent(CollisionVolume);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(GetRootComponent());

	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Idle Particle"));
	IdleParticle->SetupAttachment(GetRootComponent());

	RotationRate = 55.f;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemRotation();
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapParticle == nullptr) { return; }
	
	if (OverlapSound == nullptr) { return; }

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f));
	UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
	Destroy();
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Super::OnOverlapEnd()"));
}


void AItem::ItemRotation()
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += GetWorld()->GetDeltaSeconds() * RotationRate;
	SetActorRotation(CurrentRotation);
}