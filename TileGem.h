// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "TileGem.generated.h"

/**
 * 
 */
UCLASS()
class MATCH3FRUIT_API ATileGem : public ATile
{
	GENERATED_BODY()

public:
	ATileGem();

	bool isBursted;

	virtual void HandleTileClick() override;

	virtual void OnSwipedWith(FIntPoint OtherTileLoc) override;

	void GemBurstLogic(FIntPoint OtherTileLoc);
};
