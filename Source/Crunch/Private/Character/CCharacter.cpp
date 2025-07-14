// Fill out your copyright notice in the Description page of Project Settings.


#include "CCharacter.h"

#include "InterchangeResult.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CAbilitySystemComponent.h"
#include "GAS/UCAbilitySystemStatics.h"
#include "GAS/CAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/OverHeadStatsGauge.h"


// Sets default values
ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CAbilitySystemComponent = CreateDefaultSubobject<UCAbilitySystemComponent>("CAbilitySystemComponent");
	CAttributeSet = CreateDefaultSubobject<UCAttributeSet>("CAttributeSet");
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Overhead Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());

	BindGASChangeDelegates();
}

void ACCharacter::ServerSideInit()
{
	CAbilitySystemComponent->InitAbilityActorInfo(this, this);
	CAbilitySystemComponent->ApplyInitialEffects();
	CAbilitySystemComponent->GiveInitialAbilities();
}

void ACCharacter::ClientSideInit()
{
	CAbilitySystemComponent->ApplyInitialEffects();
}

bool ACCharacter::IsLocallyControlledByPlayer()
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ACCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverHeadStatusWidget();
}

UAbilitySystemComponent* ACCharacter::GetAbilitySystemComponent() const
{
	return CAbilitySystemComponent;
}

void ACCharacter::BindGASChangeDelegates()
{
	if (CAbilitySystemComponent)
	{
		CAbilitySystemComponent->RegisterGameplayTagEvent(UCAbilitySystemStatics::GetDeadStatTag()).AddUObject(this, &ACCharacter::DeathTagUpdated);
	}
}

void ACCharacter::DeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
	{
		StartDeathSequence();
	}
	else
	{
		Respawn();
	}
	
}

void ACCharacter::ConfigureOverHeadStatusWidget()
{
	if (!OverHeadWidgetComponent)
	{
		return;
	}
	if (IsLocallyControlledByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}
	
	UOverHeadStatsGauge* OverHeadStatsGauge = Cast<UOverHeadStatsGauge>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (OverHeadStatsGauge)
	{
		OverHeadStatsGauge->ConfigureWithASC(GetAbilitySystemComponent());
		OverHeadWidgetComponent->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(HeadStatsGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(
			HeadStatsGaugeVisibilityUpdateTimerHandle,
			this,
			&ACCharacter::UpdateHeadGaugeVisibility,
			HeadStatGaugeVisibilityCheckUpdateGap,
			true
			);
	}
}

void ACCharacter::UpdateHeadGaugeVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverHeadWidgetComponent->SetHiddenInGame(DistSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}

void ACCharacter::SetStatusGaugeEnabled(bool bEnabled)
{
	GetWorldTimerManager().ClearTimer(HeadStatsGaugeVisibilityUpdateTimerHandle);

	if (bEnabled)
	{
		ConfigureOverHeadStatusWidget();
	}
	else
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
	}
}

void ACCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage);
}

void ACCharacter::StartDeathSequence()
{
	OnDead();
	PlayDeathAnimation();
	SetStatusGaugeEnabled(false);
	
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//UE_LOG(LogTemp, Warning, TEXT("Dead"));
}

void ACCharacter::Respawn()
{
	OnRespawn();
	//UE_LOG(LogTemp, Warning, TEXT("Respawn"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);

	if (CAbilitySystemComponent)
	{
		CAbilitySystemComponent->ApplyFullStatEffect();
	}
}

void ACCharacter::OnDead()
{
}

void ACCharacter::OnRespawn()
{
}

// Called every frame
void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

