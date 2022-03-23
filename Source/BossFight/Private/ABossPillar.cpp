// Fill out your copyright notice in the Description page of Project Settings.


#include "ABossPillar.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Boss1.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AABossPillar::AABossPillar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Pillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar"));
	SetRootComponent(Pillar);

	PillarDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("pillar hitbox"));
	
	PillarDamage->SetupAttachment(Pillar);
	PillarDamage->OnComponentBeginOverlap.AddDynamic(this, &AABossPillar::Test);

	PillarDamage->OnComponentBeginOverlap.AddDynamic(this, &AABossPillar::printstring);



}

// Called when the game starts or when spawned
void AABossPillar::BeginPlay()
{
	Super::BeginPlay();
	beginloc = GetActorLocation();
	endloc = beginloc + FVector(0, 0, 1000.0f);
	up = true;
	FTimerHandle test;
	GetWorld()->GetTimerManager().SetTimer(test, this, &AABossPillar::changebool,0.5f, false);

	SetLifeSpan(2.0f);
	
}

void AABossPillar::MoveUp()
{
	FVector CurrentLoc = GetActorLocation();
	FVector Moved = CurrentLoc + FVector(0, 0, 25.0f);
	this->SetActorLocation(FMath::Lerp(CurrentLoc, Moved,1));

}

void AABossPillar::changebool()
{
	up = false;
}

void AABossPillar::printstring(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("it is overlapping"));
}

void AABossPillar::Test(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->IsA(ABoss1::StaticClass()))
	{
		FRotator LaunchDirection = OtherActor->GetActorRotation();
		LaunchDirection.Pitch = 90.0f;
		FVector LaunchVelocity = LaunchDirection.Vector() * 750;

		ACharacter* Recasted = Cast<ACharacter>(OtherActor);
		if (Recasted)
		{
			Recasted->LaunchCharacter(LaunchVelocity, true, true);
			UGameplayStatics::ApplyDamage(Recasted, 3.0f, this->GetInstigatorController(), this, PDamage);
		}
	}
	else if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("otheractor registered"));
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
		UE_LOG(LogTemp, Warning, TEXT("rootcomp got %s"),*OtherActor->GetName());
		if (MeshComp == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("is null"));
		}
		if (MeshComp) //&& OtherActor->IsRootComponentMovable()
		{
			UE_LOG(LogTemp, Warning, TEXT("rootcomponentmovable"));
			FVector upvector = OtherActor->GetActorUpVector();
			UE_LOG(LogTemp,Warning,TEXT("this is what got launched: %s"),*OtherActor->GetName())
			MeshComp->AddImpulse(upvector * MeshComp->GetMass() * 1000.0f);
		}
	}

}

// Called every frame
void AABossPillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLoc = GetActorLocation();
	while (up == true)
	{
		MoveUp();
		return;
	}
	
}

// Called to bind functionality to input
void AABossPillar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

