// Fill out your copyright notice in the Description page of Project Settings.


#include "BLaserBeam.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Boss1.h"

ABLaserBeam::ABLaserBeam()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ABLaserBeam::OnTouch);
}

void ABLaserBeam::OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->IsA(ABoss1::StaticClass()) == false)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.0f, this->GetInstigatorController(),this,LaserDamage);
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
}

void ABLaserBeam::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(0.7);
}

void ABLaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
