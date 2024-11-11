// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

#include "PaperFlipbookComponent.h" // Include the header for the Flipbook component

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Fruit  UMETA(DisplayName = "Fruit"),
	Gem    UMETA(DisplayName = "Gem"),
	None   UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ESpecificType : uint8
{
	// Fruits
	Apple    UMETA(DisplayName = "Apple"),
	Banana   UMETA(DisplayName = "Banana"),
	Orange   UMETA(DisplayName = "Orange"),
	Grape    UMETA(DisplayName = "Grape"),

	// Gems
	Watermelon UMETA(DisplayName = "Watermelon"),
	Sugarcane_Vertical UMETA(DisplayName = "Sugarcane_Vertical"),
	Sugarcane_Horizontal UMETA(DisplayName = "Sugarcane_Horizontal"),
	Pineapple UMETA(DisplayName = "Pineapple")
};

UENUM(BlueprintType)
enum class ESwipeDirection : uint8
{
	Swipe_Left     UMETA(DisplayName = "Swipe Left"),
	Swipe_Right    UMETA(DisplayName = "Swipe Right"),
	Swipe_Up       UMETA(DisplayName = "Swipe Up"),
	Swipe_Down     UMETA(DisplayName = "Swipe Down"),
	Swipe_None     UMETA(DisplayName = "No Swipe")
};

UENUM(BlueprintType)
enum class EBurstAnimationType : uint8
{
	Watermelon_Explosion UMETA(DisplayName = "Watermelon_Explosion"),
	Sugarcane_Vertical_Explosion UMETA(DisplayName = "Sugarcane_Vertical_Explosion"),
	Sugarcane_Horizontal_Explosion UMETA(DisplayName = "Sugarcane_Horizontal_Explosion"),
	Pineapple_Explosion UMETA(DisplayName = "Pineapple_Explosion")
};


UCLASS()
class MATCH3FRUIT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteComponent; // Component for rendering sprite

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComponent; // Component for rendering flipbook animation


	// Type of tile (Fruit or Gem)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	ETileType TileType;

	// Specific type of the tile (e.g., Apple, Banana, Special Gem 1, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	ESpecificType SpecificType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 rowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 colIndex;

	UFUNCTION(BlueprintCallable)
	void SetRowCol(int32 row, int32 col);
	
	UFUNCTION(BlueprintCallable)
	void SetSpecificType( ESpecificType SpecType);

	UFUNCTION(BlueprintImplementableEvent, Category = "")
	void SetSpriteAsset(ESpecificType SpecType);



	// Touch tracking variables should be static to be shared among all tile instances
	static FIntPoint InitialTouchTileLoc;
	static FIntPoint CurrentTouchTileLoc;

	static bool canSwapNow;

	// Function to handle swap
	void HandleTileSwap();
	// Function to handle click
	virtual void HandleTileClick();

	UFUNCTION(BlueprintImplementableEvent, Category = "")
	void SwipeTileAnimate(ESwipeDirection Direction);

	UFUNCTION(BlueprintImplementableEvent, Category = "")
	void moveDownAnimate(int offset);

	UFUNCTION(BlueprintImplementableEvent, Category = "")
	void BurstAnimate(EBurstAnimationType BurstAnimationType);

	virtual void OnSwipedWith(FIntPoint OtherTileLoc);


	// Functions to handle touch events
	UFUNCTION()
	void OnTouchBegin(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnTouchMoved(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnTouchEnd(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);


};
