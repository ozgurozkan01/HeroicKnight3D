// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "PlayerMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	
	StaticMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	OurMovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("Movement Component"));
	OurMovementComponent->UpdatedComponent = RootComponent; // Movement is gonna be applied to a component
															// Because of this we have to connect movement component and
															// component which is applied movement. 

	CameraRotation = FVector2D(0.f, 0.f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += CameraRotation.X; 
	SetActorRotation(CurrentRotation);

	FRotator SpringArmCurrentRotation = SpringArm->GetComponentRotation();
	SpringArmCurrentRotation.Pitch += CameraRotation.Y;
	SpringArmCurrentRotation.Pitch = FMath::Clamp(SpringArmCurrentRotation.Pitch, -85, -15);
	SpringArm->SetWorldRotation(SpringArmCurrentRotation);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMyPawn::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMyPawn::LookRight);
}

void AMyPawn::MoveForward(float inputValue)
{
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(inputValue * GetActorForwardVector());
	}
}

void AMyPawn::MoveRight(float inputValue)
{
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(inputValue * GetActorRightVector());
	}
}

void AMyPawn::LookUp(float inputValue)
{
	CameraRotation.Y = inputValue;
}

void AMyPawn::LookRight(float inputValue)
{
	CameraRotation.X = inputValue;
}

UPawnMovementComponent* AMyPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}
