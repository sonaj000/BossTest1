// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TravelPoint.h"
#include "Health.h"
#include "Boss1Controller.h"


// Sets default values
ABoss1::ABoss1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 600.0f, 0.0f);

	HealthBar = CreateDefaultSubobject<UHealth>(TEXT("HealthComp"));

	//StartMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Start Material"));
	//AlertMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Alert Material"));

}

// Called when the game starts or when spawned
void ABoss1::BeginPlay()
{
	Super::BeginPlay();
	HealthBar->OnHealthChanged.AddDynamic(this, &ABoss1::OnHealthChanged);
	
}

void ABoss1::Dead()
{
	Destroy();
}

void ABoss1::OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FTimerHandle Kill;
	if (Health <= 0.0f)
	{
		isDead = true;

		GetMovementComponent()->StopMovementImmediately();
		GetWorld()->GetTimerManager().SetTimer(Kill, this, &ABoss1::Dead, 2.0f, false);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

// Called every frame
void ABoss1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

