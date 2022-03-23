// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillSpawner.generated.h"

class USphereComponent;
class AActor;
class ABHAttack;

UCLASS()
class BOSSFIGHT_API ASkillSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile Component")
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile Component")
		UStaticMeshComponent* Ball;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType>Skill1D;

	UFUNCTION()
		void SpawnBullet();

	UPROPERTY()
		FTimerHandle Spawn;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* Beam;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* BeamTarget;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	FName Test;

	UFUNCTION()
		void RainBullet(AActor* Target);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* ImpactSound;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
