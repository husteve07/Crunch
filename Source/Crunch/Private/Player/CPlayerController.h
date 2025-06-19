// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//called on server
	void OnPossess(APawn* InPawn) override;

	//called on client
	void AcknowledgePossession(class APawn* P) override;

private:
	UPROPERTY()
	class ACPlayerCharacter* CPlayerCharacter;
};
