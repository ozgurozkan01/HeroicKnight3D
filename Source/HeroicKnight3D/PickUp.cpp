// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APickUp::APickUp()
{
	PickedCoin = 1;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == nullptr) { return; }

	AMain* MainPlayer = Cast<AMain>(OtherActor);

	if(MainPlayer == nullptr) { return; }

	if (OverlapParticle == nullptr) { return; }
	
	if (OverlapSound == nullptr) { return; }

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f));
	UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
	
	MainPlayer->IncrementCoin(PickedCoin);
	Destroy();
}

void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
