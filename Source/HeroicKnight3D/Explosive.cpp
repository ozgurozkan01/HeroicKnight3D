// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"

#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Firstly, need to check overlapped actor exist
	if(OtherActor == nullptr) { return; }

	// If overlapped actor exist, then need to try cast to Main character
	AMain* MainPlayer = Cast<AMain>(OtherActor); // It does not have to be completed

	if (MainPlayer == nullptr) { return; }

	if (OverlapParticle == nullptr) { return; }
	
	if (OverlapSound == nullptr) { return; }

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f));
	UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
	
	MainPlayer->DecrementHealth(Damage);
	Destroy();
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
