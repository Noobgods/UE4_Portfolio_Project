#include "CEnemySpawner.h"

// Sets default values
ACEnemySpawner::ACEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

