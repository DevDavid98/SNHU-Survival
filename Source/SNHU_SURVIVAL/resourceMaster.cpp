// Fill out your copyright notice in the Description page of Project Settings.


#include "resourceMaster.h"

// Sets default values
AresourceMaster::AresourceMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Used for debuggeing displays what the item is in the game 
	ResourceNameTxt = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));

	// Adds the ability to add a mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


	// Makes the component a mesh and sets the mesh
	RootComponent = Mesh;
	ResourceNameTxt->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AresourceMaster::BeginPlay()
{
	Super::BeginPlay();

	tempText = tempText.FromString(resourceName);
	ResourceNameTxt->SetText(tempText);
	
}

// Called every frame
void AresourceMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

