// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum EAttackType
{
    ROOT,
    PRIMARY_ATTACK,
    SECONDARY_ATTACK,
};

class BOSSFIGHT_API ComboTree
{
public:

	explicit ComboTree(FString AttackTag);
	~ComboTree();
    void AddChildAttack(EAttackType AttackType, ComboTree* Attack, bool Forced = false);

    ///** Removes the attack for particular type.
    //*   NOTE: This method will remove attacks recurssively.
    //*   @param AttackType Type of attack to remove.
    //*/
    void RemoveChildAttack(EAttackType AttackType);


    ComboTree* GetChildAttack(EAttackType AttackType);

    ///** Return the attack tag belongs to this attack.
    //*/
    FORCEINLINE FString GetAttackTag() const { return AttackTag; }
private:
    /** Holds the tag for the attack. Empty string if its root.
    */
    FString AttackTag;

    ///** Type of current attack
    //*/
    EAttackType Type;


    ///** Map of all branches for next attacks.
    //*/
    TMap<EAttackType, ComboTree*>Branches;

};
