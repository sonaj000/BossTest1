// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Health.h"
#include "Components/CapsuleComponent.h"
#include "CharacterAnim.h"
#include "SkillSpawner.h"
#include "AddBall.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "Math/Rotator.h"
#include "Components/CapsuleComponent.h"
#include "CollisionBox.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "SceneManagement.h"
#include "Containers/Array.h"


// Sets default values
AMCharacter::AMCharacter()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;//If this camera component is placed on a pawn, should it use the view/control rotation of the pawn where possible?

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(ArmComponent);
	CameraComp->bUsePawnControlRotation = false;

	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComp->SetupAttachment(RootComponent);

	WeaponComp2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon2"));
	WeaponComp2->SetupAttachment(RootComponent);

	Robot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Robot"));
	Robot->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<UHealth>(TEXT("HealthComp"));

	JumpHeight = 1200.0f;
	WalkingSpeed = 600.0f;
	SprintSpeed = 1200.0f;

	CanDash = true;
	isDash = false;
	inAir = false;
	DashDistance = 6000.0f;
	DashCoolDown = 0.8f;
	DashStop = 0.1f;

	WeaponAttachSocketName = "Test";

	static ConstructorHelpers::FObjectFinder<UAnimMontage>TestMontageObject(TEXT("AnimMontage'/Game/Character/reanimatedanim/combo_montage/test.test'"));
	if (TestMontageObject.Succeeded())
	{
		TestMontage = TestMontageObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Montage Succeeded"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontageObject(TEXT("AnimMontage'/Game/Character/reanimatedanim/combo_montage/Skill.Skill'"));
	if (SkillMontageObject.Succeeded())
	{
		SkillMontage = SkillMontageObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("SkillMontage Succeeded"));
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ChargeMontageObject(TEXT("AnimMontage'/Game/Character/reanimatedanim/combo_montage/ChargeAttack.ChargeAttack'"));
	if (ChargeMontageObject.Succeeded())
	{
		ChargeMontage = ChargeMontageObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("ChargeMontage Succeeded"));

	}

}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	ComboCounter = 0;
	canInterrupt = true;

	FVector BehindActor = GetActorLocation() - FVector(0.0f,-20.0f,0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(SpawnWeapon, BehindActor, FRotator::ZeroRotator, SpawnParams);
	CurrentWeapon2 = GetWorld()->SpawnActor<AWeapon>(SpawnWeapon, BehindActor, FRotator::ZeroRotator, SpawnParams);

	UE_LOG(LogTemp, Warning, TEXT("weapon spawned"));
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(WeaponComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		UE_LOG(LogTemp, Warning, TEXT("weapon attached")); //has been successfully done

		CurrentWeapon2->SetOwner(this);
		CurrentWeapon2->AttachToComponent(WeaponComp2, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		UE_LOG(LogTemp, Warning, TEXT("weapon attached")); //has been successfully done

	}



	HealthComp->OnHealthChanged.AddDynamic(this, &AMCharacter::OnHealthChanged);
	//Weapon Rotation
	StartRot = GetActorRotation();
	EndRot = CurrentWeapon->GetActorRotation();
	
	/////skill start and ends
	isSkill1 = true;
	isSkill2 = true;
	isSkill3 = true;

}

void AMCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value); //how to move the character forward. 
}


void AMCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void AMCharacter::BeginCrouch()
{
	Crouch();//function is already implemented in engine
}

void AMCharacter::EndCrouch()
{
	UnCrouch(); //
}

void AMCharacter::StopMovement()
{
	this->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AMCharacter::ResetMovement()
{
	this->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}



void AMCharacter::DoubleJump()
{
	inAir = true;
	if (DoubleJumpCounter < 2)
	{
		AMCharacter::LaunchCharacter(FVector(0.0f, 0.0f, JumpHeight),false,true);
		DoubleJumpCounter++;
	}
}

void AMCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
	inAir = false;
}

void AMCharacter::JumpAttack()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	if (inAir && controller->WasInputKeyJustPressed(FKey(EKeys::LeftMouseButton)))
	{
		PlayAnimMontage(ChargeMontage, 1.5f, FName("jump"));
		CurrentWeapon->Fire();
		CurrentWeapon2->Fire();
	}

}

void AMCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void AMCharacter::Dash()
{
	UE_LOG(LogTemp, Warning, TEXT("It has been pressed"));
	FActorSpawnParameters TempParams;
	TempParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (CanDash)
	{
		//ACollisionBox* Temp = GetWorld()->SpawnActor<ACollisionBox>(TempBox, GetActorLocation(), FRotator::ZeroRotator, TempParams);
		// this is the time slow
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;//prevents friction slowing down during dash
		//const FVector ForwardDir(GetActorForwardVector().X, GetActorForwardVector().Y, 0);
		//LaunchCharacter(ForwardDir * DashDistance, true, true);
		

		//instead of get forward vector we can use get last input vector. 
		const FVector Testing = GetLastMovementInputVector().GetSafeNormal();
		const FVector Try = this->GetActorRotation().Vector();
			//GetLastMovementInputVector().GetSafeNormal();
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Tel,GetActorLocation());
		LaunchCharacter(Testing*1000.0f, true, true);
		FVector Test = GetVelocity();
		UE_LOG(LogTemp, Warning, TEXT("Testing is %s"), *Testing.ToString());
		//LaunchCharacter(FVector(CameraComp->GetForwardVector().X,CameraComp->GetForwardVector().Y,0).GetSafeNormal()* DashDistance, true, true);
		CanDash = false;
		isDash = true;
		GetWorldTimerManager().SetTimer(CD, this, &AMCharacter::StopDash,DashStop,false);//no loop timer
	}

}

void AMCharacter::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(DC, this, &AMCharacter::ResetDash, DashCoolDown, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	
}

void AMCharacter::ResetDash()
{
	CanDash = true;
	isDash = false;
}

void AMCharacter::TimeSlow()
{
	/*spawn temporary collision hitbox(#make this another class) at player place and if it is hit, slow time and remove hitbox*/
	//set global time dilation and then set custom time dilation for character for 5 seconds. 
	FTimerHandle CharMove;
	this->CustomTimeDilation = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(CharMove, this, &AMCharacter::TimeReset, 3.0f, false);
}

void AMCharacter::TimeReset()
{
	this->CustomTimeDilation = 1.0f;
	UE_LOG(LogTemp, Warning, TEXT("Char time reset"));
}

void AMCharacter::FireStart()
{
	GetWorld()->GetTimerManager().SetTimer(TR, this, &AMCharacter::TimerReset, 1.0f, false);//delay the activation of timer reset by a second
	FString MontageSection = "start_" + FString::FromInt(ComboCounter);
	ComboPossbile = true;
	isStart = false;
	isFire = true;
	
	if (!inAir)
	{
		if (ComboCounter == 0)
		{
			CurrentWeapon->Fire();
			PlayAnimMontage(SkillMontage, 1.5f, FName(*MontageSection));
			UE_LOG(LogTemp, Warning, TEXT("should be zero Combo Counter is : %d"), ComboCounter);
			ComboCounter = 1;
			DInput();
			FTimerHandle RLerp;
			GetWorld()->GetTimerManager().SetTimer(RLerp, this, &AMCharacter::ResetLerp, 0.8f, false);
		}
		else if (ComboCounter == 1)
		{
			CurrentWeapon2->Fire();
			PlayAnimMontage(SkillMontage, 1.5f, FName(*MontageSection));
			ComboCounter += 1;
			DInput();
			FTimerHandle RLerp;
			GetWorld()->GetTimerManager().SetTimer(RLerp, this, &AMCharacter::ResetLerp, 0.8f, false);
		}
		else if (ComboCounter == 2)
		{
			if (ComboPossbile)
			{
				CurrentWeapon->Fire();
				CurrentWeapon2->Fire();
				ComboPossbile = false;
				ComboCounter += 1;
				UE_LOG(LogTemp, Warning, TEXT("Combo Counter is : %d"), ComboCounter);
				PlayAnimMontage(SkillMontage, 1.5f, FName(*MontageSection));
				DInput();
				FTimerHandle RLerp;
				GetWorld()->GetTimerManager().SetTimer(RLerp, this, &AMCharacter::ResetLerp, 0.8f, false);
				return;
			}
		}
		if (ComboCounter == 3)
		{
			CurrentWeapon->Fire();
			CurrentWeapon2->Fire();
			PlayAnimMontage(SkillMontage, 1.5f, FName(*MontageSection));
			UE_LOG(LogTemp, Warning, TEXT("Combo Counter == 4, time to reset"));
			FireReset();
			DInput();
			FTimerHandle RLerp;
			GetWorld()->GetTimerManager().SetTimer(RLerp, this, &AMCharacter::ResetLerp, 0.8f, false);
			return;
		}

	}

	
}

void AMCharacter::FireEnd()
{
	isFire = false;
}

void AMCharacter::FireReset()
{
	ComboPossbile = true;
	ComboCounter = 0;
}

void AMCharacter::TimerReset()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	if ((GetWorldTimerManager().GetTimerElapsed(TR) < 1.1f) && controller->IsInputKeyDown(FKey(EKeys::LeftMouseButton)))// time since the timer elapsed. 
		//missing one or exception where the or is if any key except left click is pressed. 
	{
		GetWorldTimerManager().ClearTimer(TR);
	}
	else
	{
		FireReset();
	}
}

void AMCharacter::ChargeAttack()
{
	//summon more bots and play montage. 
	FTimerHandle Clear;
	FString Section = "Hold";
	GetWorld()->GetTimerManager().SetTimer(STimer, this, &AMCharacter::ChargeSpawn, 0.9f, true);
	GetWorld()->GetTimerManager().SetTimer(Clear, this, &AMCharacter::ResetSpawn, 3.0f, false);
	PlayAnimMontage(ChargeMontage, 1.5f, FName(*Section));

}

void AMCharacter::ChargeSpawn()
{
	ChargeLoc; //declared in h
	ChargeLoc.Add(one);
	ChargeLoc.Add(two);
	ChargeLoc.Add(three);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//GetWorld()->SpawnActor<AWeapon>(SpawnWeapon, ChargeLoc[SpawnCounter], FRotator::ZeroRotator, SpawnParams);
	
	if (SpawnCounter < 2)
	{
		SpawnCounter += 1;
	}
	else if (SpawnCounter == 2)
	{
		SpawnCounter = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("It is spawning charge : %d"),SpawnCounter);

}

void AMCharacter::ResetSpawn()
{
	GetWorldTimerManager().ClearTimer(STimer);
}

void AMCharacter::CARelease()
{
	//fire lasers from each bots
	FString Section = "release";
	PlayAnimMontage(ChargeMontage, 1.5f, FName(*Section));
	ChargeLoc.Empty();
	UE_LOG(LogTemp, Warning, TEXT("array is emptied"));
}

void AMCharacter::ResetLerp()
{
	isStart = true;
	//this->SetActorLocation(Start);
	//CurrentWeapon->SetActorRotation(StartRot);
}

void AMCharacter::MoveFire()
{
	//this->SetActorLocation(FMath::Lerp(Start, Ending, 1));
	//CurrentWeapon->SetActorRotation(FMath::Lerp(StartRot, EndRot, 1));
}

void AMCharacter::DInput()
{
	FTimerHandle interrupt;
	APlayerController* controller = Cast<APlayerController>(GetController());
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("starting off the boolean as true: %f"), canInterrupt);
		canInterrupt = false;
		UE_LOG(LogTemp, Warning, TEXT("montage is playing and cannot be interrupted: %f"), canInterrupt);
		this->DisableInput(controller);
		UE_LOG(LogTemp, Warning, TEXT("player disable"));
		GetWorld()->GetTimerManager().SetTimer(interrupt, this, &AMCharacter::EInput, 0.4f, false);
		return;
	}

}

void AMCharacter::EInput()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	canInterrupt = true;
	this->EnableInput(controller);
}



void AMCharacter::OnHealthChanged(UHealth* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !isDead)
	{
		isDead = true;

		GetMovementComponent()->StopMovementImmediately();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//DetachFromControllerPendingDestroy();

		//SetLifeSpan(5.0f);
		UGameplayStatics::OpenLevel(this, FName("Menu_Map"), false);
		//change this to death scene later
		
	}
}

void AMCharacter::Skill1()
{
	FTimerHandle MReset;
	FVector SpawnLoc = FVector(GetActorLocation().X - 50.0f, GetActorLocation().Y + 200.0f, 300.0f);
	FRotator SpawnRot = GetActorRotation();
	FTimerHandle SkillReset;
	FTimerHandle Spawn;

	FActorSpawnParameters SpawnerParams;
	SpawnerParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (isSkill1)
	{
		AActor* Spawner = GetWorld()->SpawnActor<ASkillSpawner>(Object, SpawnLoc, SpawnRot, SpawnerParams);
		UE_LOG(LogTemp, Warning, TEXT("It has been spawned"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ObjectPretty, CurrentWeapon->GetActorLocation());
		//CurrentWeapon->SkillSpawner();

		GetWorldTimerManager().SetTimer(SkillReset, this, &AMCharacter::Skill1End, 20.0f, false);//2.0f is skill CD?
		isSkill1 = false;

		FString skill1 = "start_2";
		PlayAnimMontage(TestMontage, 1.0f, FName(*skill1));
		StopMovement();
		GetWorld()->GetTimerManager().SetTimer(MReset, this, &AMCharacter::ResetMovement, 1.2f, false);
		UE_LOG(LogTemp, Warning, TEXT("It has been played"));

	}

}

void AMCharacter::Skill1End()
{
	isSkill1 = true;
	UE_LOG(LogTemp, Warning, TEXT("the skill is now false again"));
}

void AMCharacter::Skill2()
{
	//summon pillar outline
	//get mouse location from player, and spawn pillar at location. 
	//two sections for damage application- a launch that is triggered oncomponentoverlap, and pulse damage afterwards.
	if (isSkill2)
	{
		isSkill2 = false;
		FTimerHandle Skill2T;
		GetWorld()->GetTimerManager().SetTimer(Skill2T, this, &AMCharacter::skill2end, 5.0f, false);
	}

}

void AMCharacter::skill2end()
{

	isSkill2 = true;
}

void AMCharacter::Skill3()
{
	FTimerHandle MReset;
	//liv lux cube skills
	ActorLocation = GetActorLocation();

	TArray<FHitResult> ActorsLaunched;
	const FVector BoxCoord = FVector(300.0, 300.0, 100.0f);
	FVector Center = FVector((GetActorForwardVector().X), (GetActorForwardVector().Y), 0.0f);
	FVector Final = GetActorLocation() + (Center * 200.0f);
	FRotator BoxAngle = FRotator(0.0f,0.0f,0);

	FCollisionShape BoxTime = FCollisionShape::MakeBox(BoxCoord);
	FCollisionShape MyCapsule = FCollisionShape::MakeCapsule(300.0f, 50.0f);
	

	FCollisionObjectQueryParams ObjectParams; //
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FActorSpawnParameters Skill3Spawn;
	Skill3Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (isSkill3)
	{
		isSkill3 = false;
		bool gotHit = GetWorld()->SweepMultiByObjectType(ActorsLaunched, Final, Final, FQuat::Identity, ObjectParams, MyCapsule);
		UE_LOG(LogTemp, Warning, TEXT("the number of items in there is : %d"), ActorsLaunched.Num());
		StopMovement();
		GetWorld()->GetTimerManager().SetTimer(MReset, this, &AMCharacter::ResetMovement, 1.2f, false);
		UE_LOG(LogTemp, Warning, TEXT("movement is stoppped"));
		if (gotHit)
		{
			AActor* Cube = GetWorld()->SpawnActor<AActor>(BigCube, Final + FVector(0, 0, -75.0f), BoxAngle, Skill3Spawn);
			AActor* Hole = GetWorld()->SpawnActor<AActor>(HoleEffect, Final, BoxAngle, Skill3Spawn);

			FString skill1 = "start_1";
			PlayAnimMontage(TestMontage, 1.0f, FName(*skill1));
			UE_LOG(LogTemp, Warning, TEXT("stop movement"));
			for (auto& Hit : ActorsLaunched)
			{
				//	//Add Damage to actor. 
				//	//play anim montage;
				//}
				AActor* Target = Hit.GetActor();
				UGameplayStatics::ApplyDamage(Target, 30.0f, this->GetInstigatorController(), this, DamageT);
			}
		}
		else
		{
			AActor* Cube = GetWorld()->SpawnActor<AActor>(BigCube, Final + FVector(0, 0, -80.0f), BoxAngle, Skill3Spawn);
		}
		FTimerHandle Skill3T;
		GetWorld()->GetTimerManager().SetTimer(Skill3T, this, &AMCharacter::Skill3End, 5.0f, false);

	}




}

void AMCharacter::Skill3End()
{
	isSkill3 = true;
}

// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* controller = Cast<APlayerController>(GetController());
	FVector Front = GetActorForwardVector() * 750 + FVector(0.0f, 0.0f, -200.0f);
	if (controller->IsInputKeyDown(FKey(EKeys::E)) && controller != NULL && isSkill2)
	{
		controller->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		//try and clamp z axsis
		FVector clamped = FVector(mouseLocation.X, mouseLocation.Y, FMath::Clamp(mouseLocation.Z, 180.0f, 250.0f)) + Front;
		//fix the direction the mouse location is. currently always spawns behind for some reason so print forward vector to do math.
	}
	if (controller->WasInputKeyJustReleased(FKey(EKeys::E)))
	{
		FString skill1 = "start_0";
		PlayAnimMontage(TestMontage, 1.0f, FName(*skill1));
		FActorSpawnParameters Spawnparams;
		Spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* damn = GetWorld()->SpawnActor<AActor>(BallSkill,FVector(mouseLocation.X, mouseLocation.Y, FMath::Clamp(mouseLocation.Z, 200.0f, 200.0f)) + Front - FVector(0,0,800.0f), FRotator::ZeroRotator, Spawnparams);
		UE_LOG(LogTemp, Warning, TEXT("actor is spawned: %s"), *mouseLocation.ToString());
	}
	if (isStart == false)
	{
		MoveFire();
	}
	FVector ActLoc = GetActorLocation() + GetActorForwardVector()*-30;
	one = ActLoc + FVector(-20.0f, -20.0f, 0);
	two = one + FVector(20.0f, 20.0f, 0);
	three = ActLoc + (-20, 20.0f, -20.0f);
	if (controller->IsInputKeyDown(FKey(EKeys::Tab)) && controller != NULL)
	{
		UGameplayStatics::OpenLevel(this, FName("Menu_Map"), false);
	}
	


}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMCharacter::AddControllerPitchInput);//pitch is y axis
	PlayerInputComponent->BindAxis("Turn", this, &AMCharacter::AddControllerYawInput); //z axis is yaw

	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this, &AMCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released, this, &AMCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCharacter::DoubleJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMCharacter::Walk);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMCharacter::Dash);

	PlayerInputComponent->BindAction("FireStart", IE_Pressed, this, &AMCharacter::FireStart);
	PlayerInputComponent->BindAction("JumpAttack", IE_Pressed, this, &AMCharacter::JumpAttack);

	PlayerInputComponent->BindAction("Charge", IE_Pressed, this, &AMCharacter::ChargeAttack);
	PlayerInputComponent->BindAction("CRelease", IE_Released, this, &AMCharacter::CARelease);

	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &AMCharacter::Skill1);

	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &AMCharacter::Skill2);
	PlayerInputComponent->BindAction("skill2end", IE_Released, this, &AMCharacter::Skill2);

	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &AMCharacter::Skill3);


	




}

FVector AMCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

