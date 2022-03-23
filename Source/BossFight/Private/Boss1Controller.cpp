// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TravelPoint.h"
#include "Boss1.h"
#include "Kismet/GameplayStatics.h"
#include "MCharacter.h"
#include "AddBall.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "BLaserBeam.h"
#include "Kismet/KismetMathLibrary.h"

ABoss1Controller::ABoss1Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp")));

	SightConfig->SightRadius = BossSightRadius; //max distance boss starts percieving
	SightConfig->LoseSightRadius = BossLoseSight; //max distance in which boss starts to lose sight
	SightConfig->PeripheralVisionAngleDegrees = BossFieldofView;//how far the boss can see to the side
	SightConfig->SetMaxAge(BossSightAge); // how long the sight is remembered
	
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //determines if enemies or whatever trigger this sense
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //if we utilize more than one sense 
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this,&ABoss1Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	StartMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Start Material"));
	AlertMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Alert Material"));

	bisCharacterDetected = false;
	DistanceToPlayer = 0.0;

	Ball = nullptr;

	

}

void ABoss1Controller::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss systems is set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("oops something went wrong"));
	}

}

void ABoss1Controller::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
}

void ABoss1Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StateUpdate();

}

FRotator ABoss1Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator::ZeroRotator;
	}
	return FRotator(0.0f,GetPawn()->GetActorRotation().Yaw,0.0f);
}

void ABoss1Controller::OnPawnDetected(const TArray<AActor*>&DetectedPawns)
{
	//make array public and add it on here so can be referenced in the tick function
	for (size_t i{ 0 }; i < DetectedPawns.Num(); i++)
	{
		if (DetectedPawns[i]->IsA(AMCharacter::StaticClass()))
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
			Holder = DetectedPawns[i];
			// two modes of attack based on distance to player.
		   //need to input the detected pawn and the distance to player. 
			bisCharacterDetected = true;
		}
	}
	//put a timer on this attack function
	//when the character is in range, set to true. enter attack mode. 
}


void ABoss1Controller::Attack(AActor* DPawn)
{
	ABoss1* Enemy = Cast<ABoss1>(GetPawn());
	FVector Location = Enemy->GetActorLocation() + FVector(0,0,70.0f);
	UE_LOG(LogTemp, Warning, TEXT("is attacking"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Holder->GetActorLocation());
	Enemy->SetActorRotation(Turn);

	UE_LOG(LogTemp, Warning, TEXT("changing rotation"));
	if (DistanceToPlayer < 500)
	{
		Beam = GetWorld()->SpawnActor<ABLaserBeam>(LaserAttack, Location + Enemy->GetActorForwardVector()*275, Enemy->GetActorRotation() + FRotator(0, 90, 90), SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("it spawn"));
	}
	//275
	//we can have multiple attacks here so if we want we can use switch statements or if. 
	else if (DistanceToPlayer > 500 && DistanceToPlayer  <= 1000)
	{
		Ball = GetWorld()->SpawnActor<AAddBall>(AttackBall, Location, FRotator::ZeroRotator, SpawnParams);
	}
	
}

void ABoss1Controller::AlertMat(AActor* DPawn)
{
	//enemy will change color first
	FTimerHandle AlertAttack;
	ABoss1* Enemy = Cast<ABoss1>(GetPawn());
	Enemy->GetMesh()->SetMaterial(0, AlertMaterial);

	UE_LOG(LogTemp, Warning, TEXT("Alert Material")); //has been successfully done

	////enemy will then attack on timer
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("Attack"), Holder);
	GetWorld()->GetTimerManager().SetTimer(AlertAttack, TimerDel, 1.0f, false);

	////change color of material
	FTimerDelegate ChangeMatDel;
	FTimerHandle ChangeMatH;
	ChangeMatDel.BindUFunction(this, FName("StartMat"), Holder);
	GetWorld()->GetTimerManager().SetTimer(ChangeMatH, ChangeMatDel, 1.1f, false);
	UE_LOG(LogTemp, Warning, TEXT("start Material")); //has been successfully done
}

void ABoss1Controller::StartMat(AActor* DPawn)
{
	ABoss1* Enemy = Cast<ABoss1>(GetPawn());
	Enemy->GetMesh()->SetMaterial(0, StartMaterial);
}

void ABoss1Controller::Rotate(AActor* DPawn)
{
	ABoss1* Enemy = Cast<ABoss1>(GetPawn());
	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Holder->GetActorLocation());
	Enemy->SetActorRotation(Turn);
}

void ABoss1Controller::StateUpdate()
{
	//listing all the states and calling them
	if (CurrentState == BossState::IDLE)
	{
		if (CurrentAction == BossAction::ON_ENTER)
		{
			Idle_Enter(); //if the state is idle and the action is enter idle, we enter the idle state;
		}
		else if (CurrentAction == BossAction::ON_UPDATE)
		{
			Idle_Update();
		}

	}
	else if (CurrentState == BossState::ATTACK)
	{
		if (CurrentAction == BossAction::ON_ENTER)
		{
			Attack_Enter();
		}
		if (CurrentAction == BossAction::ON_UPDATE)
		{
			Attack_Update();
		}

	}

}

void ABoss1Controller::SetBossState(BossState NewState)
{
	switch (CurrentState)
	{
	case BossState::IDLE:
		break;
	case BossState::ATTACK:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("no state implemented"), NewState)
		return;
	}
	CurrentState = NewState;
	CurrentAction = BossAction::ON_ENTER;

}


void ABoss1Controller::Idle_Enter()
{
	CurrentAction = BossAction::ON_UPDATE;
}

void ABoss1Controller::Idle_Update()
{
	ABoss1* Enemy = Cast<ABoss1>(GetPawn());
	if (Enemy->NextTravelPoint != nullptr && !bisCharacterDetected)
	{
		MoveToActor(Enemy->NextTravelPoint,2.0f);
	}
	if (bisCharacterDetected)
	{
		Idle_Exit();
	}
	
}

void ABoss1Controller::Idle_Exit()
{
	SetBossState(BossState::ATTACK);
}

void ABoss1Controller::Attack_Enter()
{
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("AlertMat"), Holder);
	GetWorld()->GetTimerManager().SetTimer(AttackHandle, TimerDel, 4.0f, true, 2.0f);
	//activate timer for attack holder
	CurrentAction = BossAction::ON_UPDATE;
}

void ABoss1Controller::Attack_Update()
{
	StopMovement();
	Rotate(Holder);
	if (Holder != nullptr)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(Holder);
	}
	if (DistanceToPlayer > BossSightRadius)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackHandle);
		bisCharacterDetected = false;
		Attack_Exit();
	}
}


void ABoss1Controller::Attack_Exit()
{

	SetBossState(BossState::IDLE);
}






