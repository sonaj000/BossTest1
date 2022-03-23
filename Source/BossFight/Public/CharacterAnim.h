// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class BOSSFIGHT_API UCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsSecondInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsThirdInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAcceptsLastInput;

public:
	UFUNCTION()
	void Fire();



	
};
