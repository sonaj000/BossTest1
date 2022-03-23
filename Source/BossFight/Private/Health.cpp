// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100;

	Health = DefaultHealth;

}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AActor* MyOwner = GetOwner();//every health component has an owner
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealth::HandleDamage);//prefix is u not A. 
	}
	
	Health = DefaultHealth;
}

void UHealth::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

}






