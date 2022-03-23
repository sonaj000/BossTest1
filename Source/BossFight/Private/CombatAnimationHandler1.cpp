// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimationHandler1.h"

ICombatAnimationHandler1::ICombatAnimationHandler1() :
	bCanInterruptAttack(false)
{
}

ICombatAnimationHandler1::~ICombatAnimationHandler1()
{
}

void ICombatAnimationHandler1::SetCharacter(ATestCharacter* BaseCharacter)
{
	Character = BaseCharacter;
}
