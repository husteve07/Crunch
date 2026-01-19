

#include "CAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Character/CCharacter.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");
	SenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight config");

	SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig->DetectionByAffiliation.bDetectNeutrals = false;

	SenseConfig->SightRadius = 1000.f;
	SenseConfig->LoseSightRadius = 1200.f;

	SenseConfig->SetMaxAge(5.f);

	SenseConfig->PeripheralVisionAngleDegrees = 100.f;

	AIPerceptionComponent->ConfigureSense(*SenseConfig);
	
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGenericTeamId(FGenericTeamId(0));

	IGenericTeamAgentInterface* PawnTeamInterface = Cast<IGenericTeamAgentInterface>(InPawn);
	if (PawnTeamInterface)
	{
		PawnTeamInterface->SetGenericTeamId(GetGenericTeamId());
	}
}

