// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class ATestCharacter;
/**
 * 
 */
class BOSSFIGHT_API ICombatAnimationHandler1
{
public:
	ICombatAnimationHandler1();

	virtual	~ICombatAnimationHandler1();

	void SetCharacter(ATestCharacter* BaseCharacter);//sets this to the character the combat animation belongs to

	virtual void OnAnimationBegin() = 0;

	virtual void OnAttackBegin() = 0;

	virtual void OnAttackEnd() = 0;

	virtual void OnAnimationEnd() = 0;

	virtual void OnAnimationInterrupted() = 0;

protected:

	ATestCharacter* Character;

	bool bCanInterruptAttack;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

};
