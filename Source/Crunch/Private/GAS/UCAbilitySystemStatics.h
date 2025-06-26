﻿

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include  "Kismet/BlueprintFunctionLibrary.h"
#include "UCAbilitySystemStatics.generated.h"
/**
 * 
 */
UCLASS()
class UCAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FGameplayTag GetBasicAttackAbilityTag();
};
