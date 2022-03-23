// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserBeam.generated.h"

class UBoxComponent;
UCLASS()
class BOSSFIGHT_API ALaserBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserBeam();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* LaserBeam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lifespan");
		float Life = 0.2;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
