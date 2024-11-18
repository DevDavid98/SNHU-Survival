// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// includes the resource header so we can use items within
#include "resourceMaster.h"
#include "playerChar.generated.h"

UCLASS()
class SNHU_SURVIVAL_API AplayerChar : public ACharacter
{

	GENERATED_BODY()

public:


	AplayerChar();


protected:


	virtual void BeginPlay() override;


public:


	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// adds the camera component to the playerActor
	// UPROPERTY makes the camera able to moved anywhere


	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UFUNCTION()
		void GiveResources(int amount, FString resourceType);


	// CREATES resources and inentory
	// UPROPERTIES make the attributes available in BP


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Wood;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Stone;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Berry;


	// Creates the array that takes an INT and string
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		TArray<int> ResourceArray;


	UPROPERTY(EditAnywhere, Category = "resources")
		TArray<FString> ResourceNameArray;


protected:

	// Created playerCharacter attributes and stats.
	// UPROPERTIES make the attributes available in BP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
		float Health;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
		float Stamina;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
		float Hunger;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
		float walkingSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stats")
		float runningSpeed;


	// END PLAYER ATTRIBUTES/STATS


	// Calls the functions and takes in a value from the mouse and called it InputValue
	void CameraTurn(float InputValue);
	void CameraUp(float InputValue);


	// Calls the functions 
	void Sprint();
	void StopSprint();
	void interact();


	// Calls the functions and takes in a value from the keyboard and called it InputValue
	void MoveForward(float InputValue);
	void MoveRight(float InputValue);


	// Set up the timer handle
	FTimerHandle playerHungerTimer;


	//create a timer counter
	int32 TimerCount = 0;


	// calls the timer function
	void HungerTimer();


	// stops the timer handle when game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};


