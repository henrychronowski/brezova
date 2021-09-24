// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HierarchicalPosePool.h"
#include "DrawDebugHelpers.h"
#include "AnimTestActor.generated.h"

#define DEBUGMESSAGE(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x), VA_ARGS));}
#define TIMEDDEBUGMESSAGE(x, y, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, x, FColor::Yellow, FString::Printf(TEXT(y), VA_ARGS));}

UCLASS()
class TOPUSKO_API AAnimTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimTestActor();

public:
	HierarchicalPosePool hierarchyPosePool;
	Hierarchy skeletonHierarchy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
