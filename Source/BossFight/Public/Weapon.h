// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UNiagaraSystem;
class ALaserBeam;

UCLASS()
class BOSSFIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<UDamageType>DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UNiagaraSystem* Laser;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	ALaserBeam* LB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ALaserBeam>LaserB;

	//lerp testing
	UPROPERTY(EditAnywhere,Category = "lerp")
	float duration = 2.0f;
	UPROPERTY(EditAnywhere,Category = "lerp")
	float WaitTime = 3.0f;

	float timeelapse = 0;

	//skill 1 moved from skillspawner
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FName Test;
	UPROPERTY()
		FTimerHandle Spawn;

	UPROPERTY()
		FTimerHandle Test2;		

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* Beam;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* BeamTarget;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* ImpactSound;
	UFUNCTION()
		void cleartimer();
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		FRotator OG;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		FRotator SpawnR;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Fire();

	UFUNCTION(BlueprintCallable, Category = "skill")
		void SkillSpawner();

	UFUNCTION(BlueprintCallable, Category = "skill")
		void RainBullet(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Rotate();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void RotateReset();






	

};
