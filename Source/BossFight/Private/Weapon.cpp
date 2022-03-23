// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "CollisionShape.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Quat.h"
#include "Math/Rotator.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "LaserBeam.h"



static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTarget = "BeamTarget";

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AWeapon::Fire()
{
	AActor* WpOwner = GetOwner(); //get the owner for the weapon which is the character
	if (WpOwner)//there is a valid owner
		UE_LOG(LogTemp, Warning, TEXT(" no Valid Owner"));
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid Owner"));
		FVector EyeLocation;
		FRotator EyeRotation;
		WpOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//function will fill these arguments;
		
		FVector ShotDirection = EyeRotation.Vector();
		///function for rotating when fire 
		Rotate();
		FCollisionQueryParams QueryParams;

		QueryParams.AddIgnoredActor(WpOwner); //ginore the owner
		QueryParams.AddIgnoredActor(this); //the weapon
		QueryParams.bTraceComplex = true;//traces complex collision so better accuracy
		QueryParams.bReturnPhysicalMaterial = true;

		TArray<FHitResult>InRadius; //contains information about the hit of the trace;
		const FVector BoxCoord = FVector(300.0, 300.0, 300.0f);
		FVector End = EyeLocation + (ShotDirection * 50.0f);

		FCollisionShape MyBox = FCollisionShape::MakeBox(BoxCoord);
		FCollisionShape MyCapsule = FCollisionShape::MakeCapsule(300.0f,50.0f);

		FVector Center = FVector((WpOwner->GetActorForwardVector().X), (WpOwner->GetActorForwardVector().Y), 0.0f);
		FVector Final = WpOwner->GetActorLocation() + (Center * 200.0f);
		
		//UE_LOG(LogTemp, Warning, TEXT("this is the location of the center of the box: %s"), *Final.ToString());

		FRotator TestRot = FRotator(270.0f, -90.0f, 270.0f);
		FCollisionObjectQueryParams ObjectParams; //
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FVector TracerEndPoint = Final; // if there is no hit then the default is in front

		bool GotHit = GetWorld()->SweepMultiByObjectType(InRadius, Final, Final, FQuat::Identity, ObjectParams, MyCapsule);
		UE_LOG(LogTemp, Warning, TEXT("the number of items in there is : %d"), InRadius.Num());
		
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		LB = GetWorld()->SpawnActor<ALaserBeam>(LaserB, this->GetActorLocation() + this->GetActorRightVector()*300, this->GetActorRotation() + FRotator(90.0f, 83, 0), SpawnParams);
		///laser part
		//if (Laser)
		//{
		//	AActor* Own = GetOwner();
		//	if (Own)
		//	{
		//		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		//		UNiagaraComponent* WeaponLaser = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Laser, MuzzleLocation, GetActorRotation() + FRotator(0.0f,90.0f,0.0f));//it works
		//		FVector AEnd = MuzzleLocation + FVector(Own->GetActorForwardVector().X, Own->GetActorForwardVector().Y, 0.0f) * 500.0f;
		//		FVector Pos = Own->GetActorLocation() + (Own->GetActorForwardVector() * 10);
		//		
		//		UE_LOG(LogTemp, Warning, TEXT("The start : %s and the end: %s and my weapon location: %s"),*MuzzleLocation.ToString(),*Pos.ToString(),*this->GetActorLocation().ToString());
		//	}
		//}
		//this part is for if something gets hit. not laser part. 
		if (GotHit)
		{
			for (auto& Hit : InRadius)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, FString::Printf(TEXT("Hit Result: %s"), *Hit.Actor->GetName()));
					UE_LOG(LogTemp, Warning, TEXT("the hit result location is: %s"),*Hit.Actor->GetActorLocation().ToString());
					if (ImpactEffect)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,Hit.Actor->GetActorLocation() + FVector(0.0f,0.0f,500.0f),TestRot,true);
					}
					TracerEndPoint = Hit.Actor->GetActorLocation();
					Hit.Actor->GetName();
					AActor* Target = Hit.GetActor();
					UGameplayStatics::ApplyDamage(Target, 5.0f, WpOwner->GetInstigatorController(), this, DamageType);
					UE_LOG(LogTemp, Warning, TEXT("Damage has been applied from auto"));

				}
			}

		}

	}
}


void AWeapon::SkillSpawner()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Beam, GetActorLocation(), GetActorRotation(), false);
	TArray<AActor*>Targets;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ABHAttack::StaticClass(), Targets);//should get all the actors in the level
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("Test"), Targets);
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("This is the total number in the array %d"), Targets.Num())

		for (AActor* Target : Targets) // for each target in targets
		{
			if (Target != NULL)
			{
				
				FTimerDelegate Test1;
				Test2;
				FTimerHandle clear;
				Test1.BindUFunction(this, FName("RainBullet"), Target);
				GetWorld()->GetTimerManager().SetTimer(Test2, Test1, 0.3f, true,1.5f);
				GetWorld()->GetTimerManager().SetTimer(clear,this,&AWeapon::cleartimer, 1.0f, false, 3.0f);
				

				//RainBullet(Target);
				UE_LOG(LogTemp, Warning, TEXT("firing beam on them"));

			}

		}
}

void AWeapon::RainBullet(AActor* Target)
{
	FVector SpawnLoc = Target->GetActorLocation();
	FRotator SpawnRot = Target->GetActorRotation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamTarget, SpawnLoc, SpawnRot);
	UE_LOG(LogTemp, Warning, TEXT("yeah it's going off"));
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, SpawnLoc);
}

void AWeapon::Rotate()
{
	OG = GetActorRotation();
	FRotator T = OG +FRotator(0, 0, -35.0f);
	this->SetActorRotation(T);
	FTimerHandle Reset;
	GetWorld()->GetTimerManager().SetTimer(Reset, this, &AWeapon::RotateReset, 0.35f, false);
}

void AWeapon::RotateReset()
{
	this->SetActorRotation(OG);
}

void AWeapon::cleartimer()
{
	GetWorld()->GetTimerManager().ClearTimer(Test2);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//lerp the location. 
	

	


}

