// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionBox.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MCharacter.h"
#include "Boss1.h"


// Sets default values
ACollisionBox::ACollisionBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACollisionBox::gotHit);

	hit = false;
	

}

// Called when the game starts or when spawned
void ACollisionBox::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACollisionBox::gotHit(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor != this && OtherActor->IsA(AMCharacter::StaticClass()) == false) // isA mcharacter function. 
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
		UE_LOG(LogTemp, Warning, TEXT("the actor overlapping is: %s"), *OtherActor->GetName());
		//get the owner? aactor * something = get owner(
		AActor* Test = this->GetOwner();
		AMCharacter* Own = Cast<AMCharacter>(Test);
		if (Own)
		{
			Own->TimeSlow();
			UE_LOG(LogTemp, Warning, TEXT("character time is speed up"));
		}
		hit = true;
		UE_LOG(LogTemp, Warning, TEXT("global dilation happening and hit is"));
		FTimerHandle forHit;
		GetWorld()->GetTimerManager().SetTimer(forHit, this, &ACollisionBox::ResetHit, 3.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("is one of the exceptions"));
	}
}

void ACollisionBox::ResetHit()
{
	hit = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("global reset is happening"))
}


// Called every frame
void ACollisionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetLifeSpan(1.0);

}

