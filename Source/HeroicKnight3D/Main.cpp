// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotation of Pawn which is applied by Controller
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MouseSensitivity = 65.f;

	// Make independent player rotation of mouse input
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate through movement direction ... 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 650.f, 0.f); // ... at this rate
	GetCharacterMovement()->JumpZVelocity = 650.f; // Designate jump velocity
	GetCharacterMovement()->AirControl = 0.3f; // Direction control rate in the air
	
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent)

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);
}

void AMain::MoveForward(float InputValue)
{
	if (Controller != nullptr && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
	}
}

void AMain::MoveRight(float InputValue)
{
	if (Controller != nullptr && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);
	}
} 

void AMain::TurnAtRate(float Rate)
{
	float TurnAtRate = Rate * MouseSensitivity * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(TurnAtRate);
}

void AMain::LookUpRate(float Rate)
{
	float LookUpRate = Rate * MouseSensitivity * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(LookUpRate);
}

