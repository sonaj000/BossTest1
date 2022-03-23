// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TravelPoint.generated.h"

class UBoxComponent;


UCLASS()
class BOSSFIGHT_API ATravelPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATravelPoint();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;//allows me to put a nonphysical transformation in the world.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* CollisionBox; // detect if we arrive

	UPROPERTY(EditAnywhere, Category = "Components")
		ATravelPoint* AwayPoint;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnActorArrived(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
