// playerChar header is needed to run the CPP file
#include "playerChar.h"
#include "pickupMaster.h"


// There is a camera needed to play the game so a cam component was added
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/SpringArmComponent.h"


// Gets the character movement component to edit air control
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"


// To have debug messaage appear in agame
#include "Engine/Engine.h"
 


AplayerChar::AplayerChar()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create and set up camera component.springarm component
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	springArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(springArm);
	Camera->bUsePawnControlRotation = true;


	playerWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player Weapon"));
	playerWeapon->SetupAttachment(GetMesh(), TEXT("hand_r_axe"));

	projectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile Arrow"));
	projectileArrow->SetupAttachment(playerWeapon, TEXT("muzzle"));


	// Add axe to player mesh and to hand socket


	// Improve air control for player
	GetCharacterMovement()->AirControl = 1;


	// Initialize ResourceNameArray
	buildingArray.SetNum(3);
	resourceArray.SetNum(3);
	ResourceNameArray.SetNum(3);
	ResourceNameArray[0] = TEXT("Wood");
	ResourceNameArray[1] = TEXT("Stone");
	ResourceNameArray[2] = TEXT("Berry");


	// Initialize player attributes/stats
	Health = 100.0f;
	Stamina = 100.0f;


	Hunger = 20.0f;
	walkingSpeed = 600.0f;
	runningSpeed = 1150.0f;

	isAttacking = false;
	hasWeapon = false;
	isAiming = false;

}



void AplayerChar::BeginPlay()
{

	Super::BeginPlay();

	// Set up the timer handle
	FTimerHandle playerStatsTimer;

	//create a timer counter
	//int32 TimerCount = 0;


	// Set the timer to the player character and triggers every 5 seconds
	GetWorld()->GetTimerManager().SetTimer(playerStatsTimer, this, &AplayerChar::HungerTimer, 2.0f, true);

	if (objWidget)
	{
		objWidget->updateBuildObj(0.0f);
		objWidget->updateMatObj(0.0f);
	}

}


// updates the health bars every tick the game is running
void AplayerChar::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	playerUI->updateBars(Health, Hunger, Stamina);


	// places the object the player built and places at endlocation
	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = Camera->GetComponentLocation();
			FVector Direction = Camera->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

}


// This function binds the user input from the keyboard and mouse to in game actions
void AplayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// this bind action detects if jump is pressed (spacebar) ("jump" can be renamed in project input settings)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);


	//Binded the letter E to the player controls and function called interact
	PlayerInputComponent->BindAction("interact", IE_Pressed, this, &AplayerChar::interact);
	//Binded the left mouse button to player contorls
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AplayerChar::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AplayerChar::StopAttack);

	//Binded the right mouse button to player contorls
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AplayerChar::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AplayerChar::notAim);


	PlayerInputComponent->BindAction("sprint", IE_Pressed, this, &AplayerChar::Sprint);
	PlayerInputComponent->BindAction("sprint", IE_Released, this, &AplayerChar::StopSprint);

	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &AplayerChar::rotateBuilding);


	// these binds detects if "A,D,W,S" is pressed (moveLeftRight & moveForwardBackward can be renamed in project input settings)
	PlayerInputComponent->BindAxis("moveRight", this, &AplayerChar::MoveRight);
	PlayerInputComponent->BindAxis("moveForward", this, &AplayerChar::MoveForward);


	// these binds detects if the mouse was moved (cameraTurn & LookUpDown can be renamed in project input settings)
	PlayerInputComponent->BindAxis("cameraTurn", this, &AplayerChar::CameraTurn);
	PlayerInputComponent->BindAxis("cameraUp", this, &AplayerChar::CameraUp);


}



// Sets up the movement for the player and takes in a value
void AplayerChar::MoveForward(float InputValue)
{

	// Gets the Fvector values and stores that value in GetActorForwardVector();
	FVector moveForward = GetActorForwardVector();


	// Set the InputValue to the value of moveForwardBackward
	AddMovementInput(moveForward, InputValue);

}



// Sets up the movement for the player and takes in a value
void AplayerChar::MoveRight(float InputValue)
{

	// Gets the Fvector values and stores that value in GetActorRightVector();
	FVector moveRight = GetActorRightVector();


	// Set the InputValue to the value of moveLeftRight
	AddMovementInput(moveRight, InputValue);

}


// Sets up the movement for the camera and takes in a value
void AplayerChar::CameraTurn(float InputValue)
{

	// Set the InputValue to the value of cameraTurn
	AddControllerYawInput(InputValue);

}


// Sets up the movement for the camera and takes in a value
void AplayerChar::CameraUp(float InputValue)
{

	// Set the InputValue to the value of lookUpDown
	AddControllerPitchInput(InputValue);

}


// Sets walk speed to running speed
void AplayerChar::Sprint()
{

	GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
	

}


// Sets walk speed to walk speed once shift is released
void AplayerChar::StopSprint()
{

	GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;

}


// Creates a line trace for object to player interaction
void AplayerChar::interact()
{

	// Checks the camera to make sure it's not null which could crash UE
	if (!Camera)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera is null!"));
		return;
	}


	// Gets the object that was hit by the line trace
	FHitResult HitResult;

	// Gets the starting location of the line trace which is the camera
	FVector StartLocation = Camera->GetComponentLocation();

	// Shoots the line trace from the starting point forward
	FVector Direction = Camera->GetForwardVector() * 800.0f;

	// Gets the end point of the line trace
	FVector EndLocation = StartLocation + Direction;

	// Creates the collision queries of the line trace on what to hit
	FCollisionQueryParams QueryParams;

	// Makes sure the line trace ignores the player character "this"
	QueryParams.AddIgnoredActor(this);

	// Makes the collision
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	if (!isBuilding)
	{
		// Creates the detection of the line trace and gives back the result
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AresourceMaster* HitResource = Cast<AresourceMaster>(HitResult.GetActor());

			if (Stamina > 5.0f)
			{
				if(HitResource)
				{
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;
					HitResource->TotalResourceAmount = HitResource->TotalResourceAmount - resourceValue;

					if (HitResource->TotalResourceAmount > resourceValue)
					{
						GiveResources(resourceValue, hitName);

						matsCollected = matsCollected + resourceValue;
						objWidget->updateMatObj(matsCollected);



						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						setStamina(-5.0f);
					}
					else
					{
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));

					}
				}

			}
			
			// HERE
			if (ApickupMaster* Pickup = Cast<ApickupMaster>(HitResult.GetActor()))
			{
				if (Pickup->bPlayerInRange)
				{
					Pickup->OnInteractedBy(this);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("You're too far away to pick this up."));
				}
			}
		}

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1.0f, 0, 1.0f);
	}
	else
	{
		isBuilding = false;


		objectsBuild = objectsBuild + 1.0f;
		objWidget->updateBuildObj(objectsBuild);
	}

}



// Makes sure the right resources are added to the array in the correct resource
void AplayerChar::GiveResources(float amount, FString resourceType)
{
	// Adds wood to index 0 in array
	if (resourceType == "Wood")
	{
		resourceArray[0] = resourceArray[0] + amount;
	}

	// Adds stones to index 1 in array
	if (resourceType == "Stone")
	{
		resourceArray[1] = resourceArray[1] + amount;
	}

	// Adds berrys to index 2 in array
	if (resourceType == "Berry")
	{
		resourceArray[2] = resourceArray[2] + amount;
	}

}


// This function subtracts from players resources when item built
void AplayerChar::updateResources(float wood, float stone, FString buildingObject)
{
	// checks the stone inventory within array
	if (wood <= resourceArray[0])
	{
		// checks the stone inventory within array
		if (stone <= resourceArray[1])
		{
			// subtracts the resources from array
			resourceArray[0] = resourceArray[0] - wood;
			resourceArray[1] = resourceArray[1] - stone;

			// increases item array by 1
			if (buildingObject == "Wall")
			{
				buildingArray[0] = buildingArray[0] + 1;
			}
			if (buildingObject == "Floor")
			{
				buildingArray[1] = buildingArray[1] + 1;
			}
			if (buildingObject == "Ceiling")
			{
				buildingArray[2] = buildingArray[2] + 1;
			}
		}
	}
}

// This function spawns in object that wants to be built in world
void AplayerChar::spawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (buildingArray[buildingID] >= 1)
		{
			// Creates a line trace and p[laces item ahead of player
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = Camera->GetComponentLocation();
			FVector Direction = Camera->GetForwardVector() * 400.0f;
			FVector EndLoication = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			// Gets the item that corresponds within the array
			buildingArray[buildingID] = buildingArray[buildingID] - 1;
			spawnedPart = GetWorld()->SpawnActor<AbuildingPart>(buildingPartClass, EndLoication, myRot, SpawnParams);
			isSuccess = true;
		}
		isSuccess = false;
	}
}



// Function makes crafted item rotate 90 degrees
void AplayerChar::rotateBuilding()
{
	if (spawnedPart)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("rotateBuilding called but spawnedPart is nullptr!"));
	}
}



// Sets up the player health status and increases when eligible
void AplayerChar::setHealth(float amount)
{
	if (Health + amount <= 100)
	{
		Health = Health + amount;
	}


}


// Sets up the player hunger status and increases when eligible
void AplayerChar::setHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}



// Sets up the player stamina status and increases when eligible
void AplayerChar::setStamina(float amount)
{
	if (Stamina + amount <= 100)
	{
		Stamina = Stamina + amount;
	}
}


// Created the hunger timer this function decreases hunger oveer time
void AplayerChar::HungerTimer()
{
	if (Hunger > 0)
	{
		setHunger(-1.0f);

	}

	setStamina(10.0f);

	if (Hunger <= 0)
	{
		setHealth(-3.0f);
	}

}



//checks if player has weapons
void AplayerChar::isArmed(bool isEquipped){

	hasWeapon = isEquipped;

	if (isEquipped == true)
	{
		//enter weapon ready animations
		UE_LOG(LogTemp, Warning, TEXT("I HAVE WEAPON NOW ENTER ATTACK STANCE"));
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I DON'T HAVE WEAPON, STAY IN IDLE"));
	}


}



//Attack functions
void AplayerChar::Attack(){
	isAttacking = true;
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));

	//play animtion if attack button is clicked

}

void AplayerChar::StopAttack(){
	isAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("STOP ATTACK"));
}

//Aim functions
void AplayerChar::Aim() {
	isAiming = true;
	UE_LOG(LogTemp, Warning, TEXT("ENTER AIM STANCE"));

	//play animtion if attack button is clicked

}

void AplayerChar::notAim() {
	isAiming = false;
	UE_LOG(LogTemp, Warning, TEXT("EXIT AIM STANCE"));
}