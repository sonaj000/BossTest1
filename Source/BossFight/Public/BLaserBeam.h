// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserBeam.h"

#include "BLaserBeam.generated.h"

/**
 * 
 */
class UBoxComponent;
class UDamage;

UCLASS()
class BOSSFIGHT_API ABLaserBeam : public ALaserBeam
{
	GENERATED_BODY()
public:
	ABLaserBeam();

	UFUNCTION()
		void OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<UDamageType>LaserDamage;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
