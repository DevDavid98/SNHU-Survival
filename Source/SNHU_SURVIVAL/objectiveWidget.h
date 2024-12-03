// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "objectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNHU_SURVIVAL_API UobjectiveWidget : public UUserWidget
{
	GENERATED_BODY()





public:

	UFUNCTION(BlueprintImplementableEvent)
		void updateMatObj(float matsCollected);

	UFUNCTION(BlueprintImplementableEvent)
		void updateBuildObj(float objectsBuilt);

};
