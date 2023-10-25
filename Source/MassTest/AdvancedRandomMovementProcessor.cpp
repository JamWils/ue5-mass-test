// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedRandomMovementProcessor.h"

#include "MassEntityTemplateRegistry.h"
#include "MassExecutionContext.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassNavigationFragments.h"
#include "MassMovementFragments.h"

UAdvancedRandomMovementProcessor::UAdvancedRandomMovementProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Hello World"));
}

void UAdvancedRandomMovementProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FMassMovementParameters>(EMassFragmentPresence::All);
}

void UAdvancedRandomMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const TConstArrayView<FTransformFragment> TransformList = Context.GetFragmentView<FTransformFragment>();
		const TArrayView<FMassMoveTargetFragment> MoveTargetList = Context.GetMutableFragmentView<FMassMoveTargetFragment>();
		const FMassMovementParameters& MovementParameters = Context.GetConstSharedFragment<FMassMovementParameters>();

		// Print a debug line with Entity Count
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
			                                 FString::Printf(TEXT("Entity Count: %d"), Context.GetNumEntities()));

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			const FTransform& Transform = TransformList[EntityIndex].GetTransform();
			FMassMoveTargetFragment& MoveTarget = MoveTargetList[EntityIndex];

			FVector CurrentLocation = Transform.GetLocation();
			FVector TargetVector = MoveTarget.Center - CurrentLocation;
			TargetVector.Z = 0.f;
			MoveTarget.DistanceToGoal = TargetVector.Size();
			MoveTarget.Forward = TargetVector.GetSafeNormal();

			if(MoveTarget.DistanceToGoal <= 20.f || MoveTarget.Center == FVector::ZeroVector)
			{
				MoveTarget.Center = FVector(FMath::RandRange(-1.f, 1.f) * 1000.f, FMath::RandRange(-1.f, 1.f) * 1000.f, CurrentLocation.Z);
				MoveTarget.DistanceToGoal = (MoveTarget.Center - Transform.GetLocation()).Size();
				MoveTarget.Forward = (MoveTarget.Center - Transform.GetLocation()).GetSafeNormal();
				MoveTarget.DesiredSpeed = FMassInt16Real(MovementParameters.DefaultDesiredSpeed);
			}
		}
	});
}