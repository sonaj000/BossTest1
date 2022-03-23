// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboTree.h"

ComboTree::ComboTree(FString AttackTag) : Type(EAttackType::ROOT),
AttackTag(AttackTag)
{
}

ComboTree::~ComboTree()
{
    // Loop through all attacks and delete them
    for (auto& attack : Branches)
    {
        delete attack.Value;
    }
}

void ComboTree::AddChildAttack(EAttackType AttackType, ComboTree* Attack, bool Forced)
{
    bool contains = Branches.Contains(AttackType);

    // If contains and force insert
    if (contains && Forced)
    {
        RemoveChildAttack(AttackType);
    }

    // Add the new attack
    if (!contains || (contains && Forced))
    {
        Attack->Type = AttackType;
        Branches.Add(AttackType, Attack);
    }
}

void ComboTree::RemoveChildAttack(EAttackType AttackType)
{
    bool contains = Branches.Contains(AttackType);
    if (contains)
    {
        // Free resource
        ComboTree* attack = Branches[AttackType];
        delete attack; // This should recursively delete all the child nodes.

        // Remove from map
        Branches.Remove(AttackType);
    }
}

ComboTree* ComboTree::GetChildAttack(EAttackType AttackType)
{
    bool contains = Branches.Contains(AttackType);
    if (contains)
    {
        return Branches[AttackType];
    }
    return nullptr;
}



