// Fill out your copyright notice in the Description page of Project Settings.


#include "BHAttack.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "UObject/UObjectBase.h"
#include "MCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss1.h"


// Sets default values
ABHAttack::ABHAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp; //parent bass

	outerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	//outerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//outer sphere which is the suck should not have physical collision.
	outerSphere->SetSphereRadius(3000);
	outerSphere->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ABHAttack::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2.5f);
	//function that stops the radial force

	
	
}


// Called every frame
void ABHAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<UPrimitiveComponent*>OverLappingComps; // makes an array with type<> and array contents
	//change to actors
	TArray<AActor*>AllActors;

	outerSphere->GetOverlappingComponents(OverLappingComps);
	for (int32 i{ 0 }; i < OverLappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverLappingComps[i];
		if (PrimComp != nullptr && PrimComp->IsSimulatingPhysics())
		{
			const float Radius = outerSphere->GetScaledSphereRadius();
			const float ForceStrength = -1500;

			PrimComp->AddRadialForce(GetActorLocation(), Radius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			UE_LOG(LogTemp, Warning, TEXT("something in the prim"));
		}
	}
	outerSphere->GetOverlappingActors(AllActors);
	for (AActor* targ : AllActors)
	{
		if (targ != this && targ != nullptr && targ->IsA(AMCharacter::StaticClass()) == false)
		{
			UStaticMeshComponent* Meshie = Cast<UStaticMeshComponent>(targ->GetRootComponent());
			if (Meshie != nullptr)
			{
				const float Radius = outerSphere->GetScaledSphereRadius();
				const float ForceStrength = -1500;

				Meshie->AddRadialForce(GetActorLocation(), Radius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
				UE_LOG(LogTemp, Warning, TEXT("actor in suck"));
			}
		}
	}



}

