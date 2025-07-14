// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CCharacter.generated.h"

UCLASS()
class CRUNCH_API ACCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACCharacter();
	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControlledByPlayer();
	//only called on server
	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**************************************************************/
	/*						Gameplay Ability					  */
	/**************************************************************/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	void BindGASChangeDelegates();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	class UCAbilitySystemComponent* CAbilitySystemComponent;

	UPROPERTY()
	class UCAttributeSet* CAttributeSet;


	/**************************************************************/
	/*						Gameplay UI     					  */
	/**************************************************************/
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	class UWidgetComponent* OverHeadWidgetComponent;
	void ConfigureOverHeadStatusWidget();

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;
	FTimerHandle HeadStatsGaugeVisibilityUpdateTimerHandle;
	void UpdateHeadGaugeVisibility();

	void SetStatusGaugeEnabled(bool bEnabled);

	/**************************************************************/
	/*						Death And Respawn Ability    		  */
	/**************************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;

	void PlayDeathAnimation();
	
	void StartDeathSequence();
	void Respawn();

	virtual void OnDead();
	virtual void OnRespawn();
};
