// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAnimationHandler1.h"

/**
 * 
 */
class BOSSFIGHT_API FCombatAnimationHandler final : public ICombatAnimationHandler1
{
public:
	FCombatAnimationHandler();

	~FCombatAnimationHandler();

    /** Called when in animation begin stage
*/
    void OnAnimationBegin() override;

    /** Called when in attack begin stage
    */
    void OnAttackBegin() override;

    /** Called when in attack end stage
    */
    void OnAttackEnd() override;

    /** Called when in animation endbegin stage
    */
    void OnAnimationEnd() override;

    void OnAnimationInterrupted() override;
};
