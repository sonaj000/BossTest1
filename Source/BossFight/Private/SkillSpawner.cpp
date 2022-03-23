// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSpawner.h"
#include "Components/SphereComponent.h"
#include "Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SceneComponent.h"
#include "BHAttack.h"
#include "Boss1.h"


// Sets default values
ASkillSpawner::ASkillSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawner"));
	RootComponent = Ball;

	Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);




}

// Called when the game starts or when spawned
void ASkillSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(Spawn, this, &ASkillSpawner::SpawnBullet, 0.5, true);
	SetLifeSpan(5.0);
}

void ASkillSpawner::SpawnBullet()
{
	//do particle system. then get all actors of specific class in level and then target them.and apply damage
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Beam, GetActorLocation(), GetActorRotation(),false);
	TArray<AActor*>Targets;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ABHAttack::StaticClass(), Targets);//should get all the actors in the level
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("Test"), Targets);
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("This is the total number in the array %d"), Targets.Num())

	for (AActor* Target : Targets) // for each target in targets
	{
		if (Target != NULL && Target->IsA(ABoss1::StaticClass()))
		{
			FTimerDelegate Test1;
			FTimerHandle Test2;
			Test1.BindUFunction(this, FName("RainBullet"), Target);
			GetWorld()->GetTimerManager().SetTimer(Test2, Test1, 1.0f, true, -5.0f);
			UE_LOG(LogTemp, Warning, TEXT("firing beam on them"));
			UGameplayStatics::ApplyDamage(Target, 5.0f, this->GetInstigatorController(), this, Skill1D);
		}

		
	}


}

void ASkillSpawner::RainBullet(AActor* Target)
{
	FVector SpawnLoc = Target->GetActorLocation();
	FRotator SpawnRot = Target->GetActorRotation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamTarget, SpawnLoc, SpawnRot);
	UE_LOG(LogTemp, Warning, TEXT("yeah it's going off"));
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, SpawnLoc);
}

// Called every frame
void ASkillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

