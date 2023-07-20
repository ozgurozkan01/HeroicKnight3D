// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"

#include "Enemy.h"
#include "GameSaveManager.h"
#include "MainPlayerController.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotation of Pawn which is applied by Controller
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Make independent player rotation of mouse input
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Arrange rotation through movement direction ...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 650.f, 0.f); // ... at this rate
	GetCharacterMovement()->JumpZVelocity = 650.f; // Designate jump velocity
	GetCharacterMovement()->AirControl = 0.1f; // rate of direction control of player in the air

	MouseSensitivity = 65.f;
	MaxHealth = 100.f;
	MaxStamina = 350.f;
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	Coin = 0;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 100.f;
	SprintingSpeed = 1000.f;
	RunningSpeed = 600.f;
	InterpSpeed = 10.f;
	DelaySeconds = 0.2f;
	FirstAttackAnimationRate = 1.5f;
	SecondAttackAnimationRate = 1.3f;
	
	bShiftKeyDown = false;
	bLMBDown = false;
	bAttacking = false;
	bInterpToEnemy = false;
	bHasCombatTarget = false;
	bMoveForward = false;
	bMoveRight = false;
	bESCDown = false;
	
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	// We cast the default controller to the MainPlayerController which we created.
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	GameSaveManager = Cast<UGameSaveManager>(UGameplayStatics::CreateSaveGameObject(UGameSaveManager::StaticClass()));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, DelaySeconds, false);


	if (!IsPlayerInFirstLevel())
	{
		GameSaveManager->LoadGameNoSwitch(this);

		if (MainPlayerController)
		{
			MainPlayerController->GameModeOnly();
		}	
	}
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAlive()) { return; }


	SetStaminaLevel();
	InterpRotationToTarget(DeltaTime);

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent)

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &AMain::ShiftKeyUp);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);
}

void AMain::SetStaminaLevel()
{
	float DeltaStamina = GetWorld()->GetDeltaSeconds() * StaminaDrainRate;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:

		if (bShiftKeyDown && (bMoveForward || bMoveRight))
		{
			if (CurrentStamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}

			else
			{
				CurrentStamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}

		else
		{
			if(CurrentStamina + DeltaStamina >= MaxStamina)
			{
				CurrentStamina = MaxStamina;
			}

			else
			{
				CurrentStamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (CurrentStamina - DeltaStamina <= 0)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				CurrentStamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}

			else
			{
				if (bMoveForward || bMoveRight)
				{
					CurrentStamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}

				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
		}

		else
		{
			if (CurrentStamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}

			CurrentStamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:

		if (bShiftKeyDown && (bMoveForward || bMoveRight))
		{

			CurrentStamina = 0.f;
		}

		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecoviring);
			CurrentStamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecoviring:

		if (CurrentStamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}

		CurrentStamina += DeltaStamina;
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}
}

void AMain::MoveForward(float InputValue)
{
	bMoveForward = false;

	if (!CanMove(InputValue)) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	if (!GetMovementComponent()->IsFalling())
	{
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
		bMoveForward = true;
	}
}

void AMain::MoveRight(float InputValue)
{
	bMoveRight = false;
	if (!CanMove(InputValue)) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	if (!GetMovementComponent()->IsFalling())
	{
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);
		bMoveRight = true;
	}
}

void AMain::TurnAtRate(float Rate)
{
	if (CanMove(Rate))
	{
		float TurnAtRate = Rate * MouseSensitivity * GetWorld()->GetDeltaSeconds();
		AddControllerYawInput(TurnAtRate);	
	}
}

void AMain::LookUpRate(float Rate)
{
	if (CanMove(Rate))
	{
		float LookUpRate = Rate * MouseSensitivity * GetWorld()->GetDeltaSeconds();
		AddControllerPitchInput(LookUpRate);
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
	//SetMovementStatus(EMovementStatus::EMS_Sprinting);
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
	//SetMovementStatus(EMovementStatus::EMS_Normal);
}

void AMain::LMBDown()
{
	bLMBDown = true;

	if (!IsAlive()) { return; }

	if (MainPlayerController)
	{
		if (MainPlayerController->bPauseMenuVisible) { return; }
	}

	else
	{
		return;
	}
	
	if (ActiveOverlappingItem)
	{
		AWeapon* CurrentWeapon = Cast<AWeapon>(ActiveOverlappingItem);

		if (CurrentWeapon == nullptr) { return; }

		CurrentWeapon->WeaponAttach(this);
		SetActiveOverlappingItem(nullptr);
	}


	else if (EquippedWeapon)
	{
		Attack();
	}
}

void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::ESCDown()
{
	bESCDown = true;

	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void AMain::ESCUp()
{
	bESCDown = true;
}

void AMain::SetMovementStatus(EMovementStatus CurrentStatus)
{
	MovementStatus = CurrentStatus;

	if (CurrentStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::IncrementCoin(int32 TakenCoin)
{
	Coin += TakenCoin;
}

void AMain::IncrementHealth(float TakenPotion)
{
	if (CurrentHealth + TakenPotion >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	else
	{
		CurrentHealth += TakenPotion;
	}
}

void AMain::IncrementAttackSpeed(float AttackSpeedPercent)
{
	FirstAttackAnimationRate += FirstAttackAnimationRate * (AttackSpeedPercent / 100.f);
	SecondAttackAnimationRate += SecondAttackAnimationRate * (AttackSpeedPercent / 100.f);
}


void AMain::Attack()
{
	if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead && !GetMovementComponent()->IsFalling())
	{
		bAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if(AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);

			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, FirstAttackAnimationRate);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, SecondAttackAnimationRate);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			default:
					;
			}
		}
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMain::Die()
{
	if (!IsAlive()) {return;}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}

	SetMovementStatus(EMovementStatus::EMS_Dead);
	/*EquippedWeapon->CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
}

void AMain::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;	
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		// If no another enemy then remove the health bar of enemy.
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);

	if (ClosestEnemy)
	{
		FVector MainPlayerLocation = GetActorLocation();
		float MinDistance = (MainPlayerLocation - ClosestEnemy->GetActorLocation()).Size();

		for (auto OverlappingActor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(OverlappingActor);

			if (Enemy)
			{	
				float DistanceToActor = (MainPlayerLocation - Enemy->GetActorLocation()).Size();

				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}

		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}

		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

bool AMain::CanMove(float InputValue)
{
	if (MainPlayerController)
	{
		return	InputValue != 0.f &&
				!bAttacking &&
				IsAlive() &&
				!MainPlayerController->bPauseMenuVisible;
				
	}

	return false;
}

bool AMain::IsPlayerInFirstLevel()
{
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	return CurrentMapName == "SunTemple";
}

void AMain::DeathEnd()
{
    GetMesh()->bPauseAnims = true; // Stop Animation
    GetMesh()->bNoSkeletonUpdate = true; // Stop updating the mesh properties
}

void AMain::SetInterpToEnemy(bool bInterpTo)
{
    bInterpToEnemy = bInterpTo;
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }

    EquippedWeapon = WeaponToSet;
}

void AMain::InterpRotationToTarget(float& DeltaTime)
{
    // this function is making to rotate character to target
    if (bInterpToEnemy && CombatTarget)
    {
        FRotator LookAtYaw = GetInterpRotationYaw(CombatTarget->GetActorLocation());
        FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

        SetActorRotation(InterpRotation);
    }
}

FRotator AMain::GetInterpRotationYaw(FVector TargetLocation)
{
    // this function is making to return difference of rotation transform between character and target.
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
    FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

    return LookAtRotationYaw;
}

float AMain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                        AActor* DamageCauser)
{
    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.f)
    {
        Die();

        if (DamageCauser)
        {
            AEnemy* Enemy = Cast<AEnemy>(DamageCauser);

            Enemy->bHasValidTarget = false;
        }
    }
	
    return DamageAmount;
}

void AMain::Jump()
{
    // Jump function in Unreal Engine is virtual. So it can be overrided.
    // We override it, cause to check character is alive
    if (!IsAlive()) { return; }

    if (MainPlayerController)
    {
	    if (MainPlayerController->bPauseMenuVisible) { return; }
    }

    else
    {
	    return;
    }
	
    Super::Jump();
}

bool AMain::IsAlive()
{
    return MovementStatus != EMovementStatus::EMS_Dead;
}
