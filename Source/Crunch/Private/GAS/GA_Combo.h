﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API UGA_Combo : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Combo();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	static FGameplayTag GetComboChangeEventTag();
	static FGameplayTag GetComboChangeEventEndTag();
	static FGameplayTag GetComboTargetEventTag();
private:
	void SetupWaitComboInputPress();

	UFUNCTION()
	void HandleInputPress(float TimeWaited);

	UFUNCTION()
	void DoDamage(FGameplayEventData Data);
	
	void TryCommitCombo();

	UPROPERTY(EditDefaultsOnly, Category = "Targetting")
	float TargetSweepSphereRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
	TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;

	TSubclassOf<UGameplayEffect> GetDamageEffectForCurrentCombo() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ComboMontage;

	UFUNCTION()
	void ComboChangedEventReceived(FGameplayEventData Data);

	FName NextComboName;
	
};



