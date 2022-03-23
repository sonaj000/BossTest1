// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatAnimationHandler.generated.h"

class AMCharacter;
class ATestCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatAnimationHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOSSFIGHT_API ICombatAnimationHandler
{
	GENERATED_BODY()
public:
	ICombatAnimationHandler(); // default constructor

	void SetCharacter(ATestCharacter* BaseCharacter);//sets this to the character the combat animation belongs to

	virtual void OnAnimationBegin() = 0;

	virtual void OnAttackBegin() = 0;

	virtual void OnAttackEnd() = 0;

	virtual void OnAnimationEnd() = 0;


	//virtual void OnAnimationInterrupted() = 0;
	/** Called when in animation begin stage and animation is interrupted
*/
	

protected:

	ATestCharacter* Character;

	bool bCanInterruptAttack;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};
