// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelPoint.h"
#include "Components/BoxComponent.h"
#include "Boss1.h"

// Sets default values
ATravelPoint::ATravelPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	CollisionBox->SetupAttachment(Root);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATravelPoint::OnActorArrived);
}

// Called when the game starts or when spawned
void ATravelPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATravelPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATravelPoint::OnActorArrived(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABoss1* Enemy = nullptr;
	if (OtherActor != nullptr)
	{
		Enemy = Cast<ABoss1>(OtherActor); //reminder enemy is now a otheractor casted to aboss1 character type. 
		if (Enemy)
		{
			Enemy->NextTravelPoint = AwayPoint;
		}
	}
}

