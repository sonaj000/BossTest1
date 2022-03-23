// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss1Controller.generated.h"

/**
 * 
 */
class AAddBall;
class ABLaserBeam;
UENUM()
enum BossState { IDLE, ATTACK };
UENUM()
enum BossAction { ON_ENTER, ON_UPDATE};

UCLASS()
class BOSSFIGHT_API ABoss1Controller : public AAIController
{
	GENERATED_BODY()
public:
	ABoss1Controller();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* inPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>&DetectedPawns);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossSightRadius = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossSightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossLoseSight = BossSightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossFieldofView = 180.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bisCharacterDetected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float DistanceToPlayer;
	///attack
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AAddBall>AttackBall;
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<ABLaserBeam>LaserAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		ABLaserBeam* Beam;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* Holder = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		AAddBall* Ball;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		FTimerHandle AttackHandle;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		FTimerHandle AttackHandle2;

protected:
	UFUNCTION()
		void Attack(AActor* DPawn);
	UFUNCTION()
		void StartMat(AActor* DPawn);

	UFUNCTION()
		void AlertMat(AActor* DPawn);

	UFUNCTION()
		void Rotate(AActor* DPawn);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool isAttack;

private:
	UPROPERTY()
		TEnumAsByte<BossState>CurrentState = BossState::IDLE;;

	UPROPERTY()
		TEnumAsByte<BossAction>CurrentAction = BossAction::ON_ENTER;;
	
	//need to use uenum instead of enum for setstate parameter

	UFUNCTION()
		void StateUpdate();

	UFUNCTION()
		void SetBossState(BossState newState);

	void Idle_Enter();
	void Idle_Update();
	void Idle_Exit();

	void Attack_Enter();
	void Attack_Update();
	void Attack_Exit();
public:
	UPROPERTY(EditAnywhere, Category = "materials")
		UMaterial* StartMaterial;

	UPROPERTY(EditAnywhere, Category = "materials")
		UMaterial* AlertMaterial;



	


	
	
	










	
};
