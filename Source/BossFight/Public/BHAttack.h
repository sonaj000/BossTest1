// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BHAttack.generated.h"

class USphereComponent;
class AAMCharacter;
class ABoss1;
UCLASS()
class BOSSFIGHT_API ABHAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABHAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components");
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components");
	USphereComponent* outerSphere; //possible second sphere aside from the actual mesh. 


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blueprints")
		TSubclassOf<class AMCharacter>Except;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blueprints")
		TArray<AActor*>MainChar;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
