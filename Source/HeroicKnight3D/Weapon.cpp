// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == nullptr) { return; }

	AMain* MainPlayer = Cast<AMain>(OtherActor);

	if(MainPlayer == nullptr) { return; }

	WeaponAttach(MainPlayer);
	
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::WeaponAttach(AMain* MainPlayer)
{
	if (MainPlayer == nullptr) { return; }

	// We made ignore to responses of these channels
	// Because, Otherwise weapon keeps going to collide every time
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// We made physics false becuase if physics keeps going simulating
	// Then we cannot control as we want. It is affected by physics while moving
	WeaponMesh->SetSimulatePhysics(false);

	// Socket which we are gonna attach weapon
	const USkeletalMeshSocket* WeaponHolderSocket = MainPlayer->GetMesh()->GetSocketByName("RightHandSocket");

	if (WeaponHolderSocket == nullptr) { return; }

	// We attached to weapon (this) to Socket (WeaponHolderSocket)
	// In this here, we add MainPlayer->GetMesh() as second parameter.
	// That refers to that the skeletal mesh which has weaponHolderSocket
	WeaponHolderSocket->AttachActor(this, MainPlayer->GetMesh());
	bRotate = false;
}
