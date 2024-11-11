// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "TileGem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

USTRUCT(BlueprintType)
struct FLastMove
{
	GENERATED_BODY()

	UPROPERTY()
	FIntPoint TileLocationA;

	UPROPERTY()
	FIntPoint TileLocationB;

	FLastMove()
		: TileLocationA(FIntPoint(-1, -1)), TileLocationB(FIntPoint(-1, -1)){}

	FLastMove(FIntPoint InLocationA, FIntPoint InLocationB)
		: TileLocationA(InLocationA), TileLocationB(InLocationB){}

	bool IsUnset() const
	{
		return TileLocationA == FIntPoint(-1, -1) && TileLocationB == FIntPoint(-1, -1);
	}
};



UCLASS()
class MATCH3FRUIT_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATile> TileFruitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATile> TileGemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridManager")
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridManager")
	int32 Columns;

	UFUNCTION(BlueprintCallable)
	void GenerateGrid();

	void HandleTileSwap(FIntPoint currTileLoc, FIntPoint swapTileLoc);

	void SwapTiles(FIntPoint currTileLoc, FIntPoint swapTileLoc);

	TArray<FIntPoint> getClusterAroundTile(FIntPoint tileLoc);

	void CheckForClusters();

	TArray<FIntPoint> checkClusterShapes(TArray<FIntPoint>& cluster);

	TArray<FIntPoint> checkSize3(TArray<FIntPoint>& cluster);
	TArray<FIntPoint> checkSize4(TArray<FIntPoint>& cluster);
	TArray<FIntPoint> checkSize5(TArray<FIntPoint>& cluster);

	void SpecialGemSpawn(TArray<FIntPoint>& cluster);

	void MoveTilesDown();

	void SpecialGemBurstAffect(FIntPoint GemTileLoc,TArray<FIntPoint> burstAffectedTiles);

	void OnTileClickReceived(int tileRowIndex, int tileColIndex);

	
	FTimerHandle timerHandler[10];
	int timerHandlerOffset=0;
	//FTimerHandle timerHandle2;

private:
	TArray<TArray<ATile*>> TileGrid;
	
	//Store last move done by user
	FLastMove LastMove;

public:

	ATile* GetTileAt(int tileRowIndex, int tileColIndex) const;


	// Singleton access method
	static AGridManager* GetInstance();

	// Function to initialize the singleton instance
	static void SetInstance(AGridManager* InGridManager);

private:
	// Static instance of the singleton
	static AGridManager* Instance;

};
