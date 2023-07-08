// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Main.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Navigation/PathFollowingComponent.h"

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

	bOverlappingCombatSphere = false;
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
	if (OtherActor)
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(AIController)
		{
			MoveToTarget(MainPlayer);
		}
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(MainPlayer)
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AMain* MainPlayer = Cast<AMain>(OtherActor);

		if(MainPlayer)
		{
			CombatTarget = MainPlayer; // Reference to pass move to target in BP 
			bOverlappingCombatSphere = true; // controller which executes MoveToTarget() function
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
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
		}
	}
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
