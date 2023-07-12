// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Main.h"
#include "MainPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->SetSphereRadius(600.f);
	
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->SetSphereRadius(100.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket")); 
	
	bOverlappingCombatSphere = false;
	bHasValidTarget = false;
		
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	Damage = 10.f;
	MinAttackDelayTime = 0.5f;
	MaxAttackDelayTime = 1.5f;
	DestroyDelay = 3.f;
	
	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
	
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		MoveToTarget(MainPlayer);
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && IsAlive())
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(MainPlayer)
		{
			bHasValidTarget = false;
			if (MainPlayer->CombatTarget == this)
			{
				MainPlayer->SetCombatTarget(nullptr);
			}
			
			
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			MainPlayer->SetHasCombatTarget(false);

			if (MainPlayer->MainPlayerController)
			{
				MainPlayer->MainPlayerController->RemoveEnemyHealthBar();
			}
			
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(MainPlayer)
		{
			MainPlayer->SetCombatTarget(this);
			MainPlayer->SetHasCombatTarget(true);
			bHasValidTarget = true;
			
			if (MainPlayer->MainPlayerController)
			{
				MainPlayer->MainPlayerController->DisplayEnemyHealthBar();
			}
			
			CombatTarget = MainPlayer; // Reference to pass move to target in BP 
			bOverlappingCombatSphere = true; // controller which executes MoveToTarget() function

			Attack();
		}
	}
}
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && IsAlive())
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(MainPlayer)
		{
			bOverlappingCombatSphere = false;

			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(MainPlayer);
				CombatTarget = nullptr;
			}
			GetWorldTimerManager().ClearTimer(AttackTimerHandle); // When attack is stoppping, then we clean up the timer.
		}
	}
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);
		if (MainPlayer)
		{
			bAttacking = true;
			if (MainPlayer->HitParticles)
			{
				const USkeletalMeshSocket* CombatSocket = GetMesh()->GetSocketByName("CombatSocket");
				FVector WeaponSocketLocation = CombatSocket->GetSocketLocation(GetMesh());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MainPlayer->HitParticles, WeaponSocketLocation, FRotator(0.f), false);
			}

			if (MainPlayer->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainPlayer->HitSound);
			}

			if (DamageType)
			{
				UGameplayStatics::ApplyDamage(MainPlayer, Damage, AIController, this, DamageType);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AEnemy::MoveToTarget(AMain* MainPlayer)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(MainPlayer);
		MoveRequest.SetAcceptanceRadius(5.f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

void AEnemy::Attack()
{
	if (!IsAlive() || !bHasValidTarget) { return; } // Enemy is dead
	
	if (AIController)
	{
		// Should not move when attacking starts
		AIController->StopMovement();
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	}

	if (!bAttacking)
	{
		// If no attacking then it would be started
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection("Attack", CombatMontage);
		}
	}

}

void AEnemy::PlaySwingSound()
{
	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	DecreaseHealth(DamageAmount);

	return DamageAmount;
}

void AEnemy::DecreaseHealth(float DamageTaken)
{
	if (CurrentHealth - DamageTaken <= 0)
	{
		Die();
	}

	CurrentHealth -= DamageTaken;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.00f);
		AnimInstance->Montage_JumpToSection("Death", CombatMontage);
	}

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
  
	// When Enemy is dead , then we close the all collisions.
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Disappear()
{
	Destroy();
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::Disappear, DestroyDelay);
}

bool AEnemy::IsAlive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::AttackEnd()
{
	bAttacking = false;

	if (bOverlappingCombatSphere)
	{
		// We add delay time when attack end for waiting for another attacking
		float AttackDelayTime = FMath::FRandRange(MinAttackDelayTime, MaxAttackDelayTime);
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::Attack, AttackDelayTime);
	}
}

void AEnemy::ActivateCombatCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCombatCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
