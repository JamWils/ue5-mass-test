// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "AdvancedRandomMovementProcessor.generated.h"

/**
 * 
 */
UCLASS()
class MASSTEST_API UAdvancedRandomMovementProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UAdvancedRandomMovementProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
	
};
