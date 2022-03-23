// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSquare.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ADamageSquare::ADamageSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SquareSkin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareSkin"));
	RootComponent = SquareSkin;
	//SquareSkin->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SquareSkin->SetSimulatePhysics(true);

	DamageSquare = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageSkin"));
	DamageSquare->SetupAttachment(SquareSkin);
	//DamageSquare->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//enable physics 


}

// Called when the game starts or when spawned
void ADamageSquare::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2.5f);
	UE_LOG(LogTemp, Warning, TEXT("square has despawned"));
}

void ADamageSquare::Disappear()
{
	Destroy();
}

// Called every frame
void ADamageSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

