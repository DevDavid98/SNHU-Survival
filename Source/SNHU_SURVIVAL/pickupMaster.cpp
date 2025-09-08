// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "playerChar.h"
#include "pickupMaster.h"

// Sets default values
ApickupMaster::ApickupMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Weapon Capsule"));
	RootComponent = WeaponCapsule;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(WeaponCapsule);

	rotatePickup = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotate Pickup"));
	rotatePickup->RotationRate = FRotator(0.f, 180.f, 0.f);

	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ApickupMaster::OnOverlapBegin);
	
	bPlayerInRange = false;
}

// Called when the game starts or when spawned
void ApickupMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ApickupMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ApickupMaster::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{
		if (OtherActor && OtherActor != this)
		{
			if (Cast<AplayerChar>(OtherActor))
			{
				bPlayerInRange = true;
				UE_LOG(LogTemp, Warning, TEXT("Player is now in range!"));
			}
		}
	}

void ApickupMaster::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
	{
		if (OtherActor && OtherActor != this)
		{
			if (Cast<AplayerChar>(OtherActor))
			{
				bPlayerInRange = false;
				UE_LOG(LogTemp, Warning, TEXT("Player is out of range!"));
			}
		}
	}

void ApickupMaster::OnInteractedBy(AplayerChar* InteractingPlayer){

	if(!InteractingPlayer || !bPlayerInRange) return;

	FName SocketToUse = NAME_None;
	FString MeshName = WeaponMesh->GetStaticMesh()->GetName();

	// Determine which socket to use
	if (MeshName.Contains("akm_47"))
	{
		SocketToUse = "hand_r_gun";
		
	}
	else if (MeshName.Contains("knife"))
	{
		SocketToUse = "hand_r_knife";
	}
	else if (MeshName.Contains("player_axe"))
	{
		SocketToUse = "hand_r_axe";
	}

	if (SocketToUse != NAME_None)
	{
		// Set the mesh
		InteractingPlayer->playerWeapon->SetStaticMesh(WeaponMesh->GetStaticMesh());

		// Attach to socket
		InteractingPlayer->playerWeapon->AttachToComponent(
			InteractingPlayer->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			SocketToUse

		);
		//sets hasWeapons to true in player script
		InteractingPlayer->isArmed(true);
		// need function to add to inventory
		//HERE
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket or mesh name not recognized: %s"), *MeshName);
	}
}
