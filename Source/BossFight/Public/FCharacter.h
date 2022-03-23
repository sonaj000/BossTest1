// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestCharacter.h"
#include "FCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BOSSFIGHT_API AFCharacter : public ATestCharacter
{
	GENERATED_BODY()
public:
	AFCharacter();

	UFUNCTION()
		void primaryAttack();

protected:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
