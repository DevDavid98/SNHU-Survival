// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


// Add thhses to make it possible to add meshes and text to display in game
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"


#include "resourceMaster.generated.h"


UCLASS()
class SNHU_SURVIVAL_API AresourceMaster : public AActor
{
	GENERATED_BODY()
	
public:	


	// Sets default values for this actor's properties
	AresourceMaster();


protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets the default attributes for the resources that can be changes in child BPs
	UPROPERTY(EditAnywhere)
		FString resourceName = "Wood";


	UPROPERTY(EditAnywhere)
		int resourceAmount = 5;


	UPROPERTY(EditAnywhere)
		int TotalResourceAmount = 100;


	UPROPERTY()
		FText tempText;


	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameTxt;


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

};
