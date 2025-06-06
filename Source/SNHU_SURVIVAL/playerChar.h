// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// includes the resource header so we can use items within
#include "resourceMaster.h"
#include "buildingPart.h"
#include "playerWidget.h"
#include "objectiveWidget.h"
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


	//player character components.	
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* playerWeapon;

	UPROPERTY(EditAnywhere)
		class UArrowComponent* projectileArrow;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* springArm;


	UFUNCTION()
		void GiveResources(float amount, FString resourceType);


	// CREATES resources and inentory
	// UPROPERTIES make the attributes available in BP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Wood;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Stone;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		int Berry;


	UPROPERTY()
		bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hasWeapon;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		UobjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuild;

	UPROPERTY()
		float matsCollected;


	// Creates the array that takes an INT and string
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "resources")
		TArray<int> resourceArray;


	UPROPERTY(EditAnywhere, Category = "resources")
		TArray<FString> ResourceNameArray;


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


	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Building Array to store info and attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> buildingArray;
	// creates a boolean
	UPROPERTY()
		bool isBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AbuildingPart> buildingPartClass;

	UPROPERTY()
		AbuildingPart* spawnedPart;


	// creates property to be used in UE5
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UplayerWidget* playerUI;


    // Calls the functions and takes in a value from the mouse and called it InputValue
	void CameraTurn(float InputValue);
	void CameraUp(float InputValue);

	// Calls the functions 
	void Sprint();
	void StopSprint();
	void interact();
	void Attack();
	void StopAttack();


	// Calls the functions and takes in a value from the keyboard and called it InputValue
	void MoveForward(float InputValue);
	void MoveRight(float InputValue);



	// creates building functions
	UFUNCTION(BlueprintCallable)
		void updateResources(float woodAmount, float stoneAmount, FString buildingObject);


	UFUNCTION(BlueprintCallable)
		void spawnBuilding(int buildingID, bool& isSuccess);

	UFUNCTION()
		void rotateBuilding();


	// creates the stamina, health and hunger functions		
	UFUNCTION(BlueprintCallable)
		void setHealth(float amount);

	UFUNCTION(BlueprintCallable)
		void setHunger(float amount);

	UFUNCTION(BlueprintCallable)
		void setStamina(float amount);


	// calls the timer function
	UFUNCTION(BlueprintCallable)
		void HungerTimer();

	UFUNCTION(BlueprintCallable)
		void isArmed(bool isEquipped);

};


