// Fill out your copyright notice in the Description page of Project Settings.
#include "SimpleRandomMovementProcessor.h"

#include "MassEntityTemplateRegistry.h"
#include "MassExecutionContext.h"
#include "MassCommonTypes.h"
#include "MassCommonFragments.h"
#include "SimpleRandomMovementTrait.h"

USimpleRandomMovementProcessor::USimpleRandomMovementProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World 4"));
}

void USimpleRandomMovementProcessor::ConfigureQueries() 
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FSimpleMovementFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void USimpleRandomMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
		{
			const TArrayView<FTransformFragment> TransformsList = Context.GetMutableFragmentView<FTransformFragment>();
			const TArrayView<FSimpleMovementFragment> MovementList = Context.GetMutableFragmentView<FSimpleMovementFragment>();
			const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

			

			for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
			{
				FTransform& Transform = TransformsList[EntityIndex].GetMutableTransform();
				FVector& MoveTarget = MovementList[EntityIndex].Target;

				FVector CurrentLocation = Transform.GetLocation();
				FVector TargetVector = MoveTarget - CurrentLocation;

				if(TargetVector.Size() <= 20.f)
				{
					MoveTarget = FVector(FMath::RandRange(-1.f, 1.f) * 1000.f, FMath::RandRange(-1.f, 1.f) * 1000.f, CurrentLocation.Z);
				} else
				{
					// if (GEngine)
					// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
					// 	                                 FString::Printf(
					// 		                                 TEXT("Entity index: %d and to Location: %s"), EntityIndex,
					// 		                                 *(CurrentLocation + TargetVector.GetSafeNormal() * 400.f *
					// 		                                 WorldDeltaTime).ToString()));
					Transform.SetLocation(CurrentLocation + TargetVector.GetSafeNormal() * 400.f * WorldDeltaTime);
				}
			}
		});
}

