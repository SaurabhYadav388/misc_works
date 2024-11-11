// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include"GridManager.h"

FIntPoint ATile::InitialTouchTileLoc(0, 0);
FIntPoint ATile::CurrentTouchTileLoc(0, 0);

bool ATile::canSwapNow = true;

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
    RootComponent = SpriteComponent;

    // Create the flipbook component and attach it to the root or sprite component
    FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
    FlipbookComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetRowCol(int32 row, int32 col)
{
    rowIndex = row;
    colIndex = col;
}

void ATile::SetSpecificType(ESpecificType SpecType)
{
    SpecificType = SpecType;
    SetSpriteAsset(SpecType);
}

void ATile::HandleTileSwap()
{
    int32 deltaX = (CurrentTouchTileLoc.X - InitialTouchTileLoc.X);
    int32 deltaY = (CurrentTouchTileLoc.Y - InitialTouchTileLoc.Y);

    FIntPoint swapTileLoc = InitialTouchTileLoc;

    if ((deltaX == 1 && deltaY == 0) || (deltaX == -1 && deltaY == 0)) {
        canSwapNow = false;
        swapTileLoc.X += deltaX;
    }
    else if ((deltaY == 1 && deltaX == 0) || (deltaY == -1 && deltaX == 0)) {
        canSwapNow = false;
        swapTileLoc.Y += deltaY;
    }
    if (swapTileLoc != InitialTouchTileLoc)
    {
        AGridManager::GetInstance()->HandleTileSwap(InitialTouchTileLoc, swapTileLoc);
    }
}

void ATile::HandleTileClick()
{
    UE_LOG(LogTemp, Warning, TEXT("handle tile click base tile"));
    
}


void ATile::OnSwipedWith(FIntPoint OtherTileLoc)
{
}



// Handle the start of a touch event
void ATile::OnTouchBegin(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
    // Record the initial touch position
    if (TouchedComponent == SpriteComponent)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Tile drag start x: %d y: %d"), rowIndex, colIndex);
        bool b = false;

        InitialTouchTileLoc.X = rowIndex;
        InitialTouchTileLoc.Y = colIndex;

        canSwapNow = true;
    }
}

// Handle the touch movement (dragging)
void ATile::OnTouchMoved(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
    // Update the current touch position as the user drags
    if (TouchedComponent == SpriteComponent && canSwapNow == true)
    {
        bool b = false;
        //UE_LOG(LogTemp, Warning, TEXT("On Drag -> x: %d y: %d "), CurrentTouchTileLoc.X, CurrentTouchTileLoc.Y);

        CurrentTouchTileLoc.X = rowIndex;
        CurrentTouchTileLoc.Y = colIndex;

        HandleTileSwap();
    }
}

// Handle the end of a touch event
void ATile::OnTouchEnd(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
    UE_LOG(LogTemp, Warning, TEXT("Touch End!"));

    CurrentTouchTileLoc.X = rowIndex;
    CurrentTouchTileLoc.Y = colIndex;

    if (InitialTouchTileLoc == CurrentTouchTileLoc)
    {
        canSwapNow = false;
        UE_LOG(LogTemp, Warning, TEXT("ON CLICK"));
        //HandleTileClick();
        AGridManager::GetInstance()->OnTileClickReceived(rowIndex, colIndex);
    }


}

