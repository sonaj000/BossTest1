// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "CombatAnimationHandler1.h"
#include "ComboTree.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATestCharacter::ATestCharacter() :
	CombatAnimationHandler(nullptr),
	ComboAttackRoot(nullptr),
	CurrentAttack(ComboAttackRoot)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComboAttackRoot = new ComboTree("");
	CurrentAttack = ComboAttackRoot;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;//If this camera component is placed on a pawn, should it use the view/control rotation of the pawn where possible?

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(ArmComponent);
	CameraComp->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CombatAnimationHandler)
	{
		CombatAnimationHandler->SetCharacter(this);
	}
	
}

void ATestCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value); //how to move the character forward. 
}


void ATestCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestCharacter::OnAnimationBegin()
{
	if (CombatAnimationHandler)
	{
		CombatAnimationHandler->OnAnimationBegin();
	}
}

void ATestCharacter::OnAttackBegin()
{
	if (CombatAnimationHandler)
	{
		CombatAnimationHandler->OnAttackBegin();
	}
}

void ATestCharacter::OnAttackEnd()
{
	if (CombatAnimationHandler)
	{
		CombatAnimationHandler->OnAttackEnd();
	}
}

void ATestCharacter::OnAnimationEnd()
{
	CurrentAttack = ComboAttackRoot;

	if (CombatAnimationHandler)
	{
		CombatAnimationHandler->OnAnimationEnd();
	}
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestCharacter::MoveRight);

}

