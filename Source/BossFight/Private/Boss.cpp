// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Health.h"
#include "MCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DamageSquare.h"
#include "GameFramework/Actor.h"
#include "BHAttack.h"
#include "SceneManagement.h"
#include "ABossPillar.h"
#include "Containers/Array.h"


// Sets default values
ABoss::ABoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossSkin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Skin1"));

	BossHealth = CreateDefaultSubobject<UHealth>(TEXT("BossHealth"));

	PhaseCounter = 0;
	
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	BossHealth->OnHealthChanged.AddDynamic(this, &ABoss::OnHealthChanged);
	//GetWorld()->GetTimerManager().SetTimer(AttackLoop, this, &ABoss::RandomAttack, 4.0f, true);
	
	
} 


void ABoss::OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//default start the boss will start attacking less frequently
	GetWorld()->GetTimerManager().SetTimer(AttackLoop, this, &ABoss::RandomAttack,4.0f, true);//4 seconds between each attack
	UE_LOG(LogTemp, Warning, TEXT("is attacking"));
	PhaseCounter = 0;
	if (Health <= 75.0f)
	{
		PhaseCounter++;
		GetWorldTimerManager().ClearTimer(AttackLoop);//clears the timer delegate for attackloop. 
		UE_LOG(LogTemp, Warning, TEXT("Timer is cleared"));
		//we want attacks more frequent now
		GetWorld()->GetTimerManager().SetTimer(AttackLoop2, this, &ABoss::RandomAttack, 2.5f, true);
		UE_LOG(LogTemp, Warning, TEXT("phase 2 is attacking"));
	}	
	if (Health <= 50.0f)
	{
		PhaseCounter++;
		//need function to make the main not have collision. 
		SpawnTest();
	}
}

void ABoss::SpawnTest()
{
	FVector Pillar1 = FVector(-1670.0f, 2480.0f, 420.0f);
	FVector Pillar2 = FVector(2320.0f, -1820.0f, 420.0f);
	FVector Pillar3 = FVector(6640.0f, 2480.0f, 420.0f);
	FVector Pillar4 = FVector(6640, 2480, 420);
	FRotator PillarRot = FRotator(0.0f);

	FActorSpawnParameters PillarParams;
	PillarParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AABossPillar>(PillarTime, Pillar1, PillarRot, PillarParams);
	GetWorld()->SpawnActor<AABossPillar>(PillarTime, Pillar2, PillarRot, PillarParams);
	GetWorld()->SpawnActor<AABossPillar>(PillarTime, Pillar3, PillarRot, PillarParams);
	GetWorld()->SpawnActor<AABossPillar>(PillarTime, Pillar4, PillarRot, PillarParams);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Wave, Pillar1, PillarRot, true);

}

void ABoss::Attack0()
{
	FVector PlayerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); //move to attack.
	FRotator PlayerRot = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();
	FVector Above = FVector(0.0f, 0.0f, 500.0f);
	FVector Spawn = Above + PlayerLoc;
	FVector OnFloor = FVector(0.0f, 0.0f, -100.0f);
	FVector Indicator = PlayerLoc + OnFloor;

	//create box volume
	// 	   spawn the box at the locaton. so include this in header file. 
	//apply point damage to actor in volume.	
	FActorSpawnParameters AttackParams;
	AttackParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Square = GetWorld()->SpawnActor<ADamageSquare>(FallingSquare, Spawn, PlayerRot, AttackParams);
	if (Square)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Precursor, Indicator, PlayerRot,true);
		//oncomponent overlap apply point damage
	}


}


void ABoss::Attack1()
{
	FVector BossLoc = GetActorLocation();
	FRotator BossRot = GetActorRotation();
	FVector Next = BossLoc + FVector(500.0f, 500.0f, 100.0f);
	FVector Other = BossLoc + FVector(-2000.0f, -2000.0f, 100.0f);

	FActorSpawnParameters AttackParams2;
	AttackParams2.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Hole = GetWorld()->SpawnActor<ABHAttack>(BlackHole, Next, BossRot, AttackParams2);
	AActor* Hole2 = GetWorld()->SpawnActor<ABHAttack>(BlackHole, Other, BossRot, AttackParams2);



	if (Hole != nullptr && Hole2 != nullptr)
	{

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Precursor, Next, BossRot, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Precursor, Other, BossRot, true);
	
		//oncomponent overlap apply point damage
	}



}

void ABoss::AttackBall()
{
	FVector SpawnPoint = GetActorLocation() + FVector(0.0f,0.0f,-100.0f);
	FRotator SpawnRot = GetActorRotation();
	FVector SpawnChange = FVector(0.0f, 0.0f, 0.0f);
	
	TArray<FVector>DirectiontoGo;
	
	DirectiontoGo.Add(FVector(-10.0f, -10.0f, 0.0f));
	DirectiontoGo.Add(FVector(-10.0f, 10.0f, 0.0f));
	DirectiontoGo.Add(FVector(10.0f, -10.0f, 0.0f));
	DirectiontoGo.Add(FVector(10.0f, 10.0f, 0.0f));
	DirectiontoGo.Add(FVector(-15.0f, 0.0f, 0.0f));
	DirectiontoGo.Add(FVector(0.0f, 15.0f, 0.0f));
	DirectiontoGo.Add(FVector(15.0f, 0.0f, 0.0f));
	DirectiontoGo.Add(FVector(0.0f, -15.0f, 0.0f));
	DirectiontoGo.Add(FVector(5.0f, -15.0f, 0.0f));
	DirectiontoGo.Add(FVector(15.0f, 5.0f, 0.0f));
	DirectiontoGo.Add(FVector(-5.0f, 15.0f, 0.0f));
	DirectiontoGo.Add(FVector(15.0f, -5.0f, 0.0f));
		


}

void ABoss::RandomAttack()
{
	int AttackN = FMath::RandRange(0, 2);//random number generator. 
	switch (AttackN)
	{
		case 0:
			Attack1();
		case 1:
			AttackBall();
		default:
			Attack0();
		break;
	}
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);





}

