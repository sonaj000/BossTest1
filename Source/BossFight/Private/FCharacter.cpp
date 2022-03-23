// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacter.h"
#include "FCombatAnimationHandler.h"
#include "ComboTree.h"


AFCharacter::AFCharacter()
{
	CombatAnimationHandler = new FCombatAnimationHandler();

	ComboTree* primary_attack = new ComboTree("start_0");

	ComboAttackRoot->AddChildAttack(EAttackType::PRIMARY_ATTACK, primary_attack);
}

void AFCharacter::primaryAttack()
{
       // Fetch the attack to perform
    ComboTree* tmpAttackNode = CurrentAttack->GetChildAttack(EAttackType::PRIMARY_ATTACK);

        // If there is valid attack available with this input
    if (tmpAttackNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("fire is working"));
        CurrentAttack = tmpAttackNode;
        PlayAnimMontage(SkillMontage, 1.0f, FName(CurrentAttack->GetAttackTag()));
    }

   

}

void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAction("primaryAttack", IE_Released, this, &AFCharacter::primaryAttack);
}


