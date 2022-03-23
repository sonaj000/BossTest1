// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

class ICombatAnimationHandler1;
class ComboTree;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class BOSSFIGHT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        USkeletalMeshComponent* WeaponComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        USpringArmComponent* ArmComponent;

    ///Movement
    void MoveForward(float value);//float axis from -1 to 1 

    void MoveRight(float value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    /** Event fired from animation blueprint at animation begin stage
    */
    UFUNCTION(BlueprintCallable, Category = "BaseCombatCharacter")
        void OnAnimationBegin();

    /** Event fired from animation blueprint at attack begin stage
    */
    UFUNCTION(BlueprintCallable, Category = "BaseCombatCharacter")
        void OnAttackBegin();

    /** Event fired from animation blueprint at attack end stage
    */
    UFUNCTION(BlueprintCallable, Category = "BaseCombatCharacter")
        void OnAttackEnd();

    /** Event fired from animation blueprint at animation end stage
    */
    UFUNCTION(BlueprintCallable, Category = "BaseCombatCharacter")
        void OnAnimationEnd();

    /** Holds the animation handler for this character.
    */
    ICombatAnimationHandler1* CombatAnimationHandler;

    UPROPERTY(EditDefaultsOnly, Category = "BaseCombatCharacter")
        UAnimMontage* SkillMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Holds the root node of the combo tree.
*/
    ComboTree* ComboAttackRoot;

    /** Holds the current attack that character is doing.
    */
    ComboTree* CurrentAttack;


};
