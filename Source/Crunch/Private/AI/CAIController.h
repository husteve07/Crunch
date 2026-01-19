// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

struct FAIStimulus;

UCLASS()
class CRUNCH_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACAIController();

	virtual void OnPossess(APawn* InPawn);
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="AI Behavior")
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Perception")
	class UAISenseConfig_Sight* SenseConfig;

	UPROPERTY(EditDefaultsOnly, Category="AI Behavior")
	FName TargetBlackboardKeyName = "Target";

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	const UObject* GetCurrentTarget() const;
	void SetCurrentTarget(AActor* NewTarget);
};



