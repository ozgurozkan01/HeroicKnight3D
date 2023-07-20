// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Enemy.h"
#include "Main.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	bWeaponParticles = false;

	WeaponState = EWeaponState::EWS_PickUp;
	Damage = 25.f;

	CoinAmountToAttach = 0;
	KillAmountToAttach = 0;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Just check raycast, sweep and overlaps, not physical things.
	CombatCollision->SetCollisionObjectType(ECC_WorldDynamic); // Set object as dynamic object. So it can be moved, pushed by other objects.
	CombatCollision->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore all channels, does not detect any collision
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap); // detect collision just in a specific channel 
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (WeaponState != EWeaponState::EWS_PickUp || OtherActor == nullptr) { return; }

	AMain* MainPlayer = Cast<AMain>(OtherActor);

	if(MainPlayer == nullptr) { return; }

	MainPlayer->SetActiveOverlappingItem(this);
	
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if (OtherActor == nullptr) { return; }

	AMain* MainPlayer = Cast<AMain>(OtherActor);

	if(MainPlayer == nullptr) { return; }

	MainPlayer->SetActiveOverlappingItem(nullptr);
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		
		if (Enemy)
		{
			if (Enemy->HitParticles)
			{
				const USkeletalMeshSocket* WeaponSocket = WeaponMesh->GetSocketByName("WeaponSocket");
				FVector WeaponSocketLocation = WeaponSocket->GetSocketLocation(WeaponMesh);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, WeaponSocketLocation, FRotator(0.f), false);
			}

			if (Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}

			if (DamageType)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstegator, this, DamageType);
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
} 

void AWeapon::ActivateCombatCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCombatCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::WeaponAttach(AMain* MainPlayer)
{
	if (MainPlayer == nullptr) { return; }

	if (!CanAttachWeapon(MainPlayer)) { return; }
	
	SetInstegator(MainPlayer->GetController());
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
	
	MainPlayer->SetEquippedWeapon(this);// Set a weapon as particular instance
	MainPlayer->SetActiveOverlappingItem(nullptr);

	if (WeaponSound)
	{
		UGameplayStatics::PlaySound2D(this, WeaponSound);
	}

	if (!bWeaponParticles)
	{
		IdleParticle->Deactivate();
	}
}

bool AWeapon::CanAttachWeapon(AMain* MainPlayer)
{
	return MainPlayer->Coin == CoinAmountToAttach && MainPlayer->KillAmount == KillAmountToAttach;
}
