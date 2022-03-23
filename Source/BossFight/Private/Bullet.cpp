// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	RootComponent = Bullet;



}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float SpeedTravel = 1000.0f;
	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * SpeedTravel * DeltaTime;

	SetActorLocation(Location);

}

