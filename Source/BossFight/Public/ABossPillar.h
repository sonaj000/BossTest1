// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ABossPillar.generated.h"

class UBoxComponent;

UCLASS()
class BOSSFIGHT_API AABossPillar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AABossPillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Pillar")
		UStaticMeshComponent* Pillar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pillar")
		UBoxComponent* PillarDamage;

	UPROPERTY(VisibleAnywhere, Category = "Move")
		FVector beginloc;

	UPROPERTY(VisibleAnywhere, Category = "Move")
		FVector endloc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pillar")
		TSubclassOf<UDamageType>PDamage;

	UPROPERTY(VisibleAnywhere, Category = "Move")
		bool up;
	UFUNCTION()
		void MoveUp();
	UFUNCTION()
		void changebool();
	UFUNCTION()
		void printstring(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Test(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
