﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SentTargetGroup.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API UAN_SentTargetGroup : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category="Gameplay Ability")
	TArray<FName> TargetSocketNames;
	UPROPERTY(EditAnywhere, Category="Gameplay Ability")
	FGameplayTag EventTag;
};
