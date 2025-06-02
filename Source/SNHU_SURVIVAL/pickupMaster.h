// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "pickupMaster.generated.h"

UCLASS()
class SNHU_SURVIVAL_API ApickupMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ApickupMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		UCapsuleComponent* WeaponCapsule;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY()
		bool bPlayerInRange;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void OnInteractedBy(AplayerChar* InteractingPlayer);

};
