// Fill out your copyright notice in the Description page of Project Settings.


#include "AddBall.h"
#include "Components/SphereComponent.h"
#include "Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "MCharacter.h"
#include "Math/TransformNonVectorized.h"
#include "Boss1.h"


// Sets default values
AAddBall::AAddBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AddBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AddBall"));
	RootComponent = AddBall;
	

	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("outerSphere"));
	OuterSphere->SetSphereRadius(100.0f);
	OuterSphere->OnComponentBeginOverlap.AddDynamic(this, &AAddBall::OnOverLap);
	OuterSphere->SetupAttachment(RootComponent);


	bisMove = false;

}

// Called when the game starts or when spawned
void AAddBall::BeginPlay()
{
	Super::BeginPlay();
	//use timer by event. 
	GetPawns();
	SetLifeSpan(2.0f);

}

void AAddBall::OnOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->IsA(ABoss1::StaticClass()) == false)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 5.0f, this->GetInstigatorController(), this, AddD);
		UE_LOG(LogTemp, Warning, TEXT("Damage has been applied"));
		FRotator LaunchDirection = OtherActor->GetActorRotation();
		LaunchDirection.Pitch = 90.0f;
		FVector LaunchVelocity = LaunchDirection.Vector() * 500;

		FVector LaunchBack = OtherActor->GetActorForwardVector();
		FVector LaunchVel = LaunchBack * -3000;
		ACharacter* Recasted = Cast<ACharacter>(OtherActor);
		if (Recasted)
		{
			Recasted->LaunchCharacter(LaunchVel, true, true);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("not either of those"));
}

// Called every frame
void AAddBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//MoveBall();
	FTimerHandle Pause;
	GetWorld()->GetTimerManager().SetTimer(Pause, this, &AAddBall::MoveBall, 0.1f, false);
}

void AAddBall::GetPawns()
{
	TArray<AActor*>Targets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMCharacter::StaticClass(), Targets);
	UE_LOG(LogTemp, Warning, TEXT("This is the total number in the array %d"), Targets.Num())
	for (AActor* Target : Targets)
	{
		EndLoc = Target->GetActorLocation();
	}
}


void AAddBall::MoveBall()
{
	FVector Location = GetActorLocation();
	SetActorLocation(FMath::VInterpTo(Location,EndLoc, 1,0.3));

}

void AAddBall::PauseBall()
{
}

//set owner in character cpp
