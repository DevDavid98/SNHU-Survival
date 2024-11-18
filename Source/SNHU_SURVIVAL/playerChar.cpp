// playerChar header is needed to run the CPP file
#include "playerChar.h"


// There is a camera needed to play the game so a cam component was added
#include "Camera/CameraComponent.h"


// Gets the character movement component to edit air control
#include "GameFramework/CharacterMovementComponent.h"


// To have debug messaage appear in agame
#include "Engine/Engine.h"
 


AplayerChar::AplayerChar()
{
	PrimaryActorTick.bCanEverTick = true;


	// Create and set up camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(GetMesh(), TEXT("head"));
	Camera->bUsePawnControlRotation = true;


	// Improve air control for player
	GetCharacterMovement()->AirControl = 1;


	// Initialize the wood, Stone and Berry ints
	Wood = 0;
	Stone = 0;
	Berry = 0;


	// Initialize ResourceNameArray
	ResourceNameArray.SetNum(3);
	ResourceNameArray[0] = TEXT("Wood");
	ResourceNameArray[1] = TEXT("Stone");
	ResourceNameArray[2] = TEXT("Berry");


	// Initialize ResourceArray and have each index be equal to the count
	ResourceArray.SetNum(3);
	ResourceArray[0] = Wood;
	ResourceArray[1] = Stone;    
	ResourceArray[2] = Berry;    



	// Initialize player attributes/stats
	Health = 100.0f;
	Stamina = 100.0f;
	Hunger = 100.0f;
	walkingSpeed = 600.0f;
	runningSpeed = 1000.0f;

}



void AplayerChar::BeginPlay()
{

	Super::BeginPlay();


	// Set the timer to the player character and triggers every 5 seconds
	GetWorld()->GetTimerManager().SetTimer(playerHungerTimer, this, &AplayerChar::HungerTimer, 5.0f, true);

}


void AplayerChar::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

// This function binds the user input from the keyboard and mouse to in game actions
void AplayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// this bind action detects if jump is pressed (spacebar) ("jump" can be renamed in project input settings)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);


	//Binded the letter E to the player controls and function called interact
	PlayerInputComponent->BindAction("interact", IE_Pressed, this, &AplayerChar::interact);


	PlayerInputComponent->BindAction("sprint", IE_Pressed, this, &AplayerChar::Sprint);
	PlayerInputComponent->BindAction("sprint", IE_Released, this, &AplayerChar::StopSprint);


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

	// Checks the camera to makes sure its not null which could crash UE
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


	// Makes sure teh line trace ignores the player character "this"
	QueryParams.AddIgnoredActor(this);


	// Makes the collision
	QueryParams.bTraceComplex = true;


	// Creaes the detection of the line trace and gives back the result
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{

		// Get the actor that was hit with the line trace
		AActor* HitActor = HitResult.GetActor();


		// Gives error that the object is not eligible for line trace 
		if (!HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitResult does not contain a valid actor."));
			return;
		}


		// If eligible then display the object hit in the log
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit actor: %s"), *HitActor->GetName());


		// Makes sure that the actor is or object hit by line trace is apart of the resourceMaster class 
		AresourceMaster* HitResrouce = Cast<AresourceMaster>(HitActor);
		if (!HitResrouce)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor is not of type AresourceMaster."));
			return;
		}


		// Checks the targets resource amount of the object hit and if valid
		if (HitResrouce->resourceAmount <= 0 || HitResrouce->TotalResourceAmount <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid resource data: resourceAmount: %d, TotalResourceAmount: %d"),
				HitResrouce->resourceAmount,
				HitResrouce->TotalResourceAmount);
			return;
		}


		// Gets the name of the resouce
		FString hitName = HitResrouce->resourceName;


		// Gets the resource amount
		int resourceValue = HitResrouce->resourceAmount;


		// Gets the total resource amount and subtracts by the value
		HitResrouce->TotalResourceAmount -= resourceValue;


		// Checks to make sure the resource has some left
		if (HitResrouce->TotalResourceAmount > 0)
		{
			GiveResources(resourceValue, hitName);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Resource Collected"));
			}
		}
		else
		{
			// Once the resource runs out then delte the object
			UE_LOG(LogTemp, Warning, TEXT("Resource depleted. Destroying actor."));
			HitResrouce->Destroy();
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit anything."));
	}
}





// Created the hunger timer functions that is aattached to the player
void AplayerChar::HungerTimer()
{

	// Adds 1 to the TimerCount which is set to 0 in the header file
	TimerCount++;


	// If the count is greater than or equal to 1 start decreaseing hunger by 10
	if (TimerCount >= 5)
	{
		Hunger -= 10;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Player Hunger: %.2f"), Hunger));
	}


	if (Hunger <= 0)
	{

		// When hunger reaches 0. health decreases by 10.
		Health -= 10;


		// Displays health value for debugging reseaons
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Health: %.2f"), Health));


		if (Health <= 0)
		{

			Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health reached 0 (YOU ARE DEAD)"), Health));

		}

		// When hunger reaches 0. stamina is reduced/set to 50 and then decreases by 10
		Stamina -= 10;


		// Displays stamina value for debugging reseaons
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player Stamina: %.2f"), Stamina));


	}

	
	// If the player health is 0 then clear the timer/stop the timer
	if(Health <= 0)
	{

		GetWorld()->GetTimerManager().ClearTimer(playerHungerTimer);

	}

}


// Ends the timer
void AplayerChar::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);


	GetWorld()->GetTimerManager().ClearTimer(playerHungerTimer);
}


// Makes sure the right resources are added to the array in the correct resource
void AplayerChar::GiveResources(int amount, FString resourceType)
{
	if (resourceType == "Wood")
	{
		ResourceArray[0] = ResourceArray[0] + amount;
	}
	if (resourceType == "Stone")
	{
		ResourceArray[1] = ResourceArray[1] + amount;
	}
	if (resourceType == "Berry")
	{
		ResourceArray[2] = ResourceArray[2] + amount;
	}

}
