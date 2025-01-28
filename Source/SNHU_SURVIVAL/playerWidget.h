// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "playerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNHU_SURVIVAL_API UplayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// creates a function to be used in UE5 called update bars
	UFUNCTION(BlueprintImplementableEvent)
		void updateBars(float Health1, float Hunger1, float Stamina1);
	
};
