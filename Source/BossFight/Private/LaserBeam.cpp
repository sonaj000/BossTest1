// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaserBeam::ALaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserBeam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser Beam"));
	SetRootComponent(LaserBeam);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(LaserBeam);

}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Life);
	
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

