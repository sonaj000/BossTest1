// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AddBall.generated.h"

class USphereComponent;
UCLASS()
class BOSSFIGHT_API AAddBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAddBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AddBall Component")
		USphereComponent* HitBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AddBall Component")
		USphereComponent* OuterSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AddBall Component")
		UStaticMeshComponent* AddBall;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AddBall Component")
		UStaticMeshComponent* OuterBall;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle MoveOrb;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MoveBall")
		bool bisMove;

	UFUNCTION()
		void OnOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MoveBall")
		FVector EndLoc;

	UFUNCTION()
		void GetPawns();

	UFUNCTION()
		void MoveBall();

	UFUNCTION()
		void PauseBall();

	UPROPERTY(VisibleAnywhere, Category = "Damage")
		TSubclassOf<UDamageType>AddD;
};
