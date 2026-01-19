

#include "CAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
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
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::TargetPerceptionUpdated);
	
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

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

void ACAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (!GetCurrentTarget())
		{
			//UE_LOG(LogTemp, Display, TEXT("TargetPerceptionUpdated(), NoCurrentTarget, setting"));
			SetCurrentTarget(TargetActor);
		}
		//UE_LOG(LogTemp, Display, TEXT("TargetPerceptionUpdated(), CurrentTarget exists"));

	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("TargetPerceptionUpdated(), NothingSensed"));

		if (GetCurrentTarget() == TargetActor)
			SetCurrentTarget(nullptr);
	}
}

const UObject* ACAIController::GetCurrentTarget() const
{
	const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent)
	{
		//UE_LOG(LogTemp, Display, TEXT("GetCurrentTarget(), blackBoardFound"));
		return GetBlackboardComponent()->GetValueAsObject(TargetBlackboardKeyName);
	}
	//UE_LOG(LogTemp, Display, TEXT("GetCurrentTarget(), blackBoard NOt Found"));

	return nullptr;
}

void ACAIController::SetCurrentTarget(AActor* NewTarget)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent)
		return;

	if (NewTarget)
	{
		BlackboardComponent->SetValueAsObject(TargetBlackboardKeyName, NewTarget);
	}
	else
	{
		BlackboardComponent->ClearValue(TargetBlackboardKeyName);
	}
}

