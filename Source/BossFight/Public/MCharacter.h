// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatAnimationHandler.h"
#include "MCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class UParticleSystem;
class UHealth;
class ASkillSpawner;
class AAddBall;
class ACollisionBox;
class ICombatAnimationHandler;
class ComboTree;
class UDamage;

UCLASS()
class BOSSFIGHT_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType>DamageT;

	///Movement
	void MoveForward(float value);//float axis from -1 to 1 

	void MoveRight(float value);

	void BeginCrouch();

	void EndCrouch();

	UFUNCTION()
	void StopMovement();

	UFUNCTION()
	void ResetMovement();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* WeaponComp2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* Robot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* ArmComponent;
	////////////////////////////////////// custom functions
	//jump
	UPROPERTY(BlueprintReadOnly, Category = "Jump")
		bool inAir;
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void DoubleJump();

	int DoubleJumpCounter;

	void virtual Landed(const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight;
	/////////////////jump attack
	UFUNCTION(BlueprintCallable, Category = "Jump")
		void JumpAttack();

	////////////////////////////////////sprint
	UFUNCTION()
		void Sprint();
	UFUNCTION()
		void Walk();
	UPROPERTY(EditAnywhere)
		float SprintSpeed;
	UPROPERTY(EditAnywhere)
		float WalkingSpeed;
	////////////////////////////////////Dash
	UFUNCTION()
		void Dash();
	UPROPERTY(EditAnywhere)
		float DashDistance;
	UPROPERTY(EditAnywhere)
		float DashCoolDown;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Dodge")
		bool CanDash;

	UPROPERTY(EditAnywhere)
		float DashStop;
	UPROPERTY()
		FTimerHandle CD;
	UPROPERTY()
		FTimerHandle DC;
	UFUNCTION()
		void StopDash();
	UFUNCTION()
		void ResetDash();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	UParticleSystem* Tel;

	//////////////////////////////////////fire
	UPROPERTY()
	AWeapon* CurrentWeapon;

	UPROPERTY()
		AWeapon* CurrentWeapon2;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* twist;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeapon>SpawnWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<AWeapon>SpawnWeapon2;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleAnywhere)
		int ComboCounter;

	UPROPERTY(VisibleAnywhere)
	bool ComboPossbile;

	UPROPERTY(VisibleAnywhere)
		bool canInterrupt;

	UFUNCTION()
	void FireStart();

	UFUNCTION()
	void FireEnd();

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool isFire;

	UFUNCTION()
	void FireReset();

	UFUNCTION()
		void TimerReset();// reset the timer to see if we need to reset the combo since there is no input since last. 
	UPROPERTY()
		FTimerHandle TR; //timer handle for the timer reset
	UPROPERTY()
		FTimerHandle FR; // timer handle for firereset when u reset the combo
	UPROPERTY()
	APlayerController* controllers;
	////////////////////Charge Attack;
	UPROPERTY(BlueprintReadOnly,Category = "Attack")
		TArray<FVector>ChargeLoc;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
		int SpawnCounter;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
		FVector one;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
		FVector two;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
		FVector three;
	UFUNCTION()
		void ChargeAttack();
	UFUNCTION()
		void ChargeSpawn();
	UPROPERTY()
		FTimerHandle STimer;
	UFUNCTION()
		void ResetSpawn();
	UFUNCTION()
		void CARelease();

	////rotate weapon
	UPROPERTY(EditAnywhere, Category = "lerp")
		FRotator StartRot;

	UPROPERTY(EditAnywhere, Category = "lerp")
		FRotator EndRot;

	UPROPERTY(EditAnywhere, Category = "lerp")
		bool isStart;
	UFUNCTION()
		void ResetLerp();
	UFUNCTION()
		void MoveFire();

	////////////////

	/////////////////////////////////////////// health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealth* HealthComp;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool isDead;

	UFUNCTION()
		void OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//////////////////////////////////////animation montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* TestMontage;

	//skills
	UFUNCTION()
	void Skill1(); //summon orb that explodes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* ChargeMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool isSkill1;
	
	UFUNCTION()
	void Skill1End();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASkillSpawner>Object;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* ObjectPretty;

	/// <summary>
	/// //////////////////skill 2
	/// </summary>
	UFUNCTION()
	void Skill2(); //spawn from weapon?

	UFUNCTION()
		void skill2end();
	UPROPERTY(BlueprintReadOnly, Category = "skill2")
		FVector mouseLocation;
	
	UPROPERTY(BlueprintReadOnly, Category = "skill2")
		FVector mouseDirection;

	UPROPERTY(BlueprintReadOnly, Category = "skill2")
		bool isSkill2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<AActor>BallSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* oursphere;

	UFUNCTION()
		void Skill3();// liv lux cube
	UFUNCTION()
		void Skill3End();
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool isSkill3;


	UPROPERTY(VisibleAnywhere, Category = "Location")
		FVector ActorLocation;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		TSubclassOf<AActor>BigCube;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor>HoleEffect;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge")
		bool isDash;

	UFUNCTION()
		void DInput();
	UFUNCTION()
		void EInput();

	/////////////////////slow motion for dash
	UFUNCTION()
		void TimeSlow();
	UFUNCTION()
		void TimeReset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TimeSlow")
		TSubclassOf<ACollisionBox>TempBox;

	

};
