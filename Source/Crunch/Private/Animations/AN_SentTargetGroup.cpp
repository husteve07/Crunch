﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SentTargetGroup.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAN_SentTargetGroup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp)
		return;
	if (TargetSocketNames.Num() <= 1)
		return;
	if (!MeshComp->GetOwner() || !UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
		return;

	FGameplayEventData Data;

	for (int i = 1; i< TargetSocketNames.Num(); i++)
	{
		//heap allocation
		FGameplayAbilityTargetData_LocationInfo* LocationInfo= new FGameplayAbilityTargetData_LocationInfo();

		FVector StartLocation = MeshComp->GetSocketLocation(TargetSocketNames[i-1]);
		FVector EndLocation = MeshComp->GetSocketLocation(TargetSocketNames[i]);

		LocationInfo->SourceLocation.LiteralTransform.SetLocation(StartLocation);
		LocationInfo->TargetLocation.LiteralTransform.SetLocation(EndLocation);

		Data.TargetData.Add(LocationInfo);
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Data);
}
