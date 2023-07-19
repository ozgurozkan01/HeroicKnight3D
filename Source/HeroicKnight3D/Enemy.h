// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.generated.h"

class UBoxComponent;
class AAIController;
class USphereComponent;
class AMain;
class USoundCue;

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Dead			UMETA(DisplayName = "Dead"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class HEROICKNIGHT3D_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UBoxComponent* CombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	AAIController* AIController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | Sound")
	USoundCue* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | Sound")
	USoundCue* SwingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items | AI")
	UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	AMain* CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TSubclassOf<UDamageType> DamageType;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float MinAttackDelayTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float MaxAttackDelayTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	bool bOverlappingCombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float DestroyDelay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	bool bAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	bool bHasValidTarget;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus CurrentStatus) { EnemyMovementStatus = CurrentStatus; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetMaxWalkSpeed(float CurrentMaxWalkSpeed) { GetCharacterMovement()->MaxWalkSpeed = CurrentMaxWalkSpeed; }

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(AMain* MainPlayer);
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void ActivateCombatCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateCombatCollision();
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Die(AActor* Causer);
	void Disappear();
	bool IsAlive();
	
};	