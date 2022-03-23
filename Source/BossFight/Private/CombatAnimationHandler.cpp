// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimationHandler.h"

// Add default functionality here for any ICombatAnimationHandler functions that are not pure virtual.

ICombatAnimationHandler::ICombatAnimationHandler() : bCanInterruptAttack(false)
{
}


void ICombatAnimationHandler::SetCharacter(ATestCharacter* BaseCharacter)
{
	Character = BaseCharacter;
}
