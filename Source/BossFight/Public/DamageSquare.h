// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageSquare.generated.h"

class UBoxComponent;

UCLASS()
class BOSSFIGHT_API ADamageSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageSquare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Boss")
		UStaticMeshComponent* SquareSkin;

	UPROPERTY()
	UBoxComponent* DamageSquare;

	UFUNCTION()
	void Disappear();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
