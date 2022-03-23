// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss1.generated.h"

class ATravelPoint;
class UHealth;

UCLASS()
class BOSSFIGHT_API ABoss1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss1();

	UPROPERTY(EditAnywhere, Category = "Travel points")
		ATravelPoint* NextTravelPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void Dead();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		UHealth* HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		bool isDead;

	UFUNCTION()
		void OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//UPROPERTY(EditAnywhere, Category = "materials")
	//	UMaterial* StartMaterial;

	//UPROPERTY(EditAnywhere, Category = "materials")
	//	UMaterial* AlertMaterial;

};
