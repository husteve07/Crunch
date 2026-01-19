// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class CRUNCH_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACAIController();

	virtual void OnPossess(APawn* InPawn);

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Perception")
	class UAISenseConfig_Sight* SenseConfig;
};
