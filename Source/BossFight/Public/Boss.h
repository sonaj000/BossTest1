// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss.generated.h"

class USphereComponent;
class UHealth;
class UBoxComponent;
class ADamageSquare;
class ABHAttack;
class AABossPillar;

UCLASS()
class BOSSFIGHT_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Boss")
	UStaticMeshComponent* BossSkin;

	UPROPERTY(VisibleAnywhere, Category = "Boss")
	USphereComponent* BossRadius;

	///////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealth* BossHealth;
	
	int PhaseCounter;

	UFUNCTION()
	void OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY()
	FTimerHandle AttackLoop;
	////////////////////////////////////spawn test 
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AABossPillar>PillarTime;

	UFUNCTION()
	void SpawnTest();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UParticleSystem* Wave;

	/// ////////////////attack 0
	UFUNCTION()
	void Attack0();

	UPROPERTY()
	UBoxComponent* DamageBox;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ADamageSquare>FallingSquare;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UParticleSystem* Precursor;

	UPROPERTY()
	FTimerHandle Despawn;


	////////////////////attack 1
	UPROPERTY()
		FTimerHandle AttackLoop2;

	UFUNCTION()
		void Attack1();
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ABHAttack>BlackHole;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UParticleSystem* Explosion;

	UPROPERTY()
		FTimerHandle Explode;
	/// <summary>
	/// //////////////////////////////////
	/// </summary>
	UFUNCTION()
		void AttackBall();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UParticleSystem* BallAura;
	UPROPERTY()
		FTimerHandle FireBall;

	UFUNCTION()
		void RandomAttack();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		FVector direction;

};
