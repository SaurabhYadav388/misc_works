// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Rows = 8; // Default rows
    Columns = 8; // Default columns

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
    GenerateGrid();
	
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::GenerateGrid()
{

    int32 initGridValues[8][8] = {
        {0, 1, 7, 0, 1, 2, 3, 0},
        {1, 2, 0, 1, 2, 3, 1, 0},
        {2, 0, 1, 0, 3, 1, 0, 1},
        {0, 1, 2, 3, 0, 2, 3, 0},
        {1, 2, 0, 1, 2, 0, 1, 0},
        {2, 0, 1, 0, 3, 1, 2, 1},
        {3, 2, 0, 1, 2, 3, 0, 1},
        {0, 1, 3, 0, 1, 2, 3, 2}
    };


    TileGrid.SetNum(Rows);
    for (int32 Row = 0; Row < Rows; Row++)
    {
        TileGrid[Row].SetNum(Columns);
        for (int32 Col = 0; Col < Columns; Col++)
        {
            FVector Location(Row * 140.0f, Col * 140.0f, 0);
            FRotator Rotator(0.0, 90.0, 90.0);

            ATile* NewTile;
            if(initGridValues[Row][Col]<=3)
                NewTile = GetWorld()->SpawnActor<ATile>(TileFruitClass, Location, Rotator);
            else
                NewTile = GetWorld()->SpawnActor<ATile>(TileGemClass, Location, Rotator);
            
            TileGrid[Row][Col] = NewTile;

            NewTile->SetRowCol(Row, Col);

            switch (initGridValues[Row][Col])
            {
                case 0: NewTile->SetSpecificType(ESpecificType::Apple);
                    break;
                case 1: NewTile->SetSpecificType(ESpecificType::Banana);
                    break;
                case 2: NewTile->SetSpecificType(ESpecificType::Orange);
                    break;
                case 3: NewTile->SetSpecificType(ESpecificType::Grape);
                    break;

                case 4: NewTile->SetSpecificType(ESpecificType::Watermelon);
                    break;
                case 5: NewTile->SetSpecificType(ESpecificType::Sugarcane_Vertical);
                    break;
                case 6: NewTile->SetSpecificType(ESpecificType::Sugarcane_Horizontal);
                    break;
                case 7: NewTile->SetSpecificType(ESpecificType::Pineapple);
                    break;
                default:
                    UE_LOG(LogTemp, Warning, TEXT("default log"));
                
            }


        }
    }

}

void AGridManager::HandleTileSwap(FIntPoint currTileLoc, FIntPoint swapTileLoc)
{

    UE_LOG(LogTemp, Warning, TEXT("curr.x:%d curr.y:%d  swp.x:%d swp.y:%d"),currTileLoc.X,currTileLoc.Y,swapTileLoc.X,swapTileLoc.Y);

    SwapTiles(currTileLoc,swapTileLoc);   

    ATile* currTileObj = TileGrid[currTileLoc.X][currTileLoc.Y];
    ATile* swapTileObj = TileGrid[swapTileLoc.X][swapTileLoc.Y];

    GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++)%10], [this, currTileObj, swapTileObj, currTileLoc, swapTileLoc] {

        //If both tile are normal fruit
        if (currTileObj->TileType == ETileType::Fruit && swapTileObj->TileType == ETileType::Fruit)
        {
            if (getClusterAroundTile(currTileLoc).Num() >= 3 || getClusterAroundTile(swapTileLoc).Num() >= 3)
            {
                //validSwap
                LastMove = FLastMove(currTileLoc, swapTileLoc);//store this move as last move
                //call cluster elimination code with moving tile down 
                GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::CheckForClusters, 0.25f, false);
            }
            else
            {
                //undo swap
                GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, currTileLoc, swapTileLoc]() {
                    SwapTiles(currTileLoc, swapTileLoc);
                    }, 0.25f, false);
            }

        }
        //if both are gem
        else if (currTileObj->TileType == ETileType::Gem && swapTileObj->TileType == ETileType::Gem)
        {
            LastMove = FLastMove(currTileLoc, swapTileLoc);//store this move as last move

            //swaptonj is the tile moved by drag
            swapTileObj->OnSwipedWith(FIntPoint(swapTileObj->rowIndex, swapTileObj->colIndex));
            //currTileObj->OnSwipedWith(FIntPoint(currTileObj->rowIndex, currTileObj->colIndex));

            //
            GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::MoveTilesDown, 0.5f, false);
        }
        else
        {
            //one of them is special gem 
            LastMove = FLastMove(currTileLoc, swapTileLoc);//store this move as last move

            if(currTileObj->TileType == ETileType::Gem)
                currTileObj->OnSwipedWith(FIntPoint(swapTileObj->rowIndex, swapTileObj->colIndex));
            
            else
                swapTileObj->OnSwipedWith(FIntPoint(currTileObj->rowIndex, currTileObj->colIndex));
            
            //
            GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::MoveTilesDown, 0.5f, false);
        }

    }, 0.25f, false);
    
}

void AGridManager::SwapTiles(FIntPoint currTileLoc, FIntPoint swapTileLoc)
{
    ATile* currTileObj = TileGrid[currTileLoc.X][currTileLoc.Y];
    ATile* swapTileObj = TileGrid[swapTileLoc.X][swapTileLoc.Y];

    if (currTileLoc.X == swapTileLoc.X)
    {
        if (currTileLoc.Y < swapTileLoc.Y)
        {
            UE_LOG(LogTemp, Warning, TEXT("left to right"));
            currTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Right);
            swapTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Left);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("right to left"));
            currTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Left);
            swapTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Right);
        }
    }
    else
    {
        if (currTileLoc.X < swapTileLoc.X)
        {
            UE_LOG(LogTemp, Warning, TEXT("up to down"));
            currTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Down);
            swapTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Up);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("down to up"));
            currTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Up);
            swapTileObj->SwipeTileAnimate(ESwipeDirection::Swipe_Down);
        }
    }

    int32 currRow = currTileObj->rowIndex;
    int32 currCol = currTileObj->colIndex;
    int32 swapRow = swapTileObj->rowIndex;
    int32 swapCol = swapTileObj->colIndex;

    currTileObj->rowIndex = swapRow;
    currTileObj->colIndex = swapCol;

    swapTileObj->rowIndex = currRow;
    swapTileObj->colIndex = currCol;

    //swap map values
    TileGrid[currTileLoc.X][currTileLoc.Y] = swapTileObj;
    TileGrid[swapTileLoc.X][swapTileLoc.Y] = currTileObj;
}

TArray<FIntPoint> AGridManager::getClusterAroundTile(FIntPoint tileLoc)
{
    TArray<FIntPoint> cluster;

    TArray<TArray<bool>> visited;
    visited.SetNum(Rows);
    for (int32 i = 0; i < Rows; ++i)
    {
        visited[i].Init(false, Columns);
    }


    TQueue<FIntPoint> queue;

    if (TileGrid[tileLoc.X][tileLoc.Y] == nullptr)
        return cluster;

    queue.Enqueue(tileLoc);
    visited[tileLoc.X][tileLoc.Y] = true;

    FString tempStr = FString("");

    while (!queue.IsEmpty())
    {
        FIntPoint currTileLoc = *queue.Peek();
        queue.Pop();


        cluster.Add(currTileLoc);

        //TODO: need to add only for fruit match
        //left
        FIntPoint leftTileId(currTileLoc.X, currTileLoc.Y - 1);
        if (currTileLoc.Y > 0 && TileGrid[leftTileId.X][leftTileId.Y] != nullptr)
        {
            if (visited[leftTileId.X][leftTileId.Y] == false && TileGrid[currTileLoc.X][currTileLoc.Y]->SpecificType == TileGrid[leftTileId.X][leftTileId.Y]->SpecificType)
            {
                visited[leftTileId.X][leftTileId.Y] = true;
                queue.Enqueue(leftTileId);
            }
        }
        //right
        FIntPoint rightTileId(currTileLoc.X, currTileLoc.Y + 1);
        if (currTileLoc.Y < Columns - 1 && TileGrid[rightTileId.X][rightTileId.Y] != nullptr)
        {
            
            if (visited[rightTileId.X][rightTileId.Y] == false && TileGrid[currTileLoc.X][currTileLoc.Y] != nullptr && TileGrid[currTileLoc.X][currTileLoc.Y]->SpecificType == TileGrid[rightTileId.X][rightTileId.Y]->SpecificType)
            {
                visited[rightTileId.X][rightTileId.Y] = true;
                queue.Enqueue(rightTileId);
            }
        }
        //up
        FIntPoint upTileId(currTileLoc.X - 1, currTileLoc.Y);
        if (currTileLoc.X > 0 && TileGrid[upTileId.X][upTileId.Y] != nullptr)
        {
            
            if (visited[upTileId.X][upTileId.Y] == false && TileGrid[currTileLoc.X][currTileLoc.Y]->SpecificType == TileGrid[upTileId.X][upTileId.Y]->SpecificType)
            {
                visited[upTileId.X][upTileId.Y] = true;
                queue.Enqueue(upTileId);
            }
        }
        //down
        FIntPoint dowmTileId(currTileLoc.X + 1, currTileLoc.Y);
        if (currTileLoc.X < 8 - 1 && TileGrid[dowmTileId.X][dowmTileId.Y] != nullptr)
        {
            if (visited[dowmTileId.X][dowmTileId.Y] == false && TileGrid[currTileLoc.X][currTileLoc.Y]->SpecificType == TileGrid[dowmTileId.X][dowmTileId.Y]->SpecificType)
            {
                visited[dowmTileId.X][dowmTileId.Y] = true;
                queue.Enqueue(dowmTileId);
            }
        }
    }

    cluster.Sort([](const FIntPoint& A, const FIntPoint& B)
        {
            if (A.X != B.X)
            {
                return A.X < B.X;  // Sort by X first
            }
            return A.Y < B.Y;      // If X is the same, sort by Y
        });

    cluster = checkClusterShapes(cluster);

    return cluster;
}

void AGridManager::CheckForClusters()
{
    bool checkLoop = false;
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Columns; col++)
        {
            if (TileGrid[row][col] != nullptr && TileGrid[row][col]->TileType == ETileType::Fruit)//check custer only for fruits
            {
                FIntPoint currTileLoc(row, col);
                TArray<FIntPoint> cluster = getClusterAroundTile(currTileLoc);
                cluster = checkClusterShapes(cluster);

                if (cluster.Num() >= 3)
                {
                    
                    checkLoop = true;
                    for (auto tileIndex : cluster)
                    {
                        TileGrid[tileIndex.X][tileIndex.Y]->Destroy();//TODO check if right
                        TileGrid[tileIndex.X][tileIndex.Y] = nullptr;

                    }
                    //call special gem spawn check
                    SpecialGemSpawn(cluster);

                }

            }
        }
    }

    //refresh the last move
    LastMove = FLastMove();
    if (checkLoop)
        GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::MoveTilesDown, 0.5f, false);
}

TArray<FIntPoint> AGridManager::checkClusterShapes(TArray<FIntPoint>& cluster)
{
    int32 clusterSize = cluster.Num();

    if (clusterSize < 3) {
        cluster.Empty();
        return cluster;
    }
    else if (clusterSize == 3) {
        return checkSize3(cluster);
    }
    else if (clusterSize == 4) {
        return checkSize4(cluster);
    }
    else if (clusterSize == 5) {
        return checkSize5(cluster);
    }
    else {
        return checkSize5(cluster);
    }
}

TArray<FIntPoint> AGridManager::checkSize3(TArray<FIntPoint>& cluster)
{
    TArray<FIntPoint> validCluster;

    for (int point = 0; point < cluster.Num(); point++) {

        //horizontal
        if (cluster.Contains(FIntPoint(cluster[point].X, (cluster[point].Y) + 1)) && cluster.Contains(FIntPoint(cluster[point].X, (cluster[point].Y) + 2))) {
            //UE_LOG(LogTemp, Warning, TEXT("Horizontal 3!"));
            validCluster.Add(FIntPoint(cluster[point]));
            validCluster.Add(FIntPoint(cluster[point].X, (cluster[point].Y) + 1));
            validCluster.Add(FIntPoint(cluster[point].X, (cluster[point].Y) + 2));

            return validCluster;
        }

        //vertical
        if (cluster.Contains(FIntPoint((cluster[point].X) + 1, cluster[point].Y)) && cluster.Contains(FIntPoint((cluster[point].X + 2), cluster[point].Y))) {
            //UE_LOG(LogTemp, Warning, TEXT("Vertical 3!"));

            validCluster.Add(FIntPoint(cluster[point]));
            validCluster.Add(FIntPoint(cluster[point].X + 1, cluster[point].Y));
            validCluster.Add(FIntPoint(cluster[point].X + 2, cluster[point].Y));

            return validCluster;
        }
    }
    return validCluster;
}

TArray<FIntPoint> AGridManager::checkSize4(TArray<FIntPoint>& cluster)
{
    TArray<FIntPoint> validCluster;

    for (int point = 0; point < cluster.Num(); point++) {

        //horizontal
        if (cluster.Contains(FIntPoint(cluster[point].X, cluster[point].Y + 1)) && cluster.Contains(FIntPoint(cluster[point].X, cluster[point].Y + 2)) && cluster.Contains(FIntPoint(cluster[point].X, cluster[point].Y + 3))) {
            validCluster.Add(FIntPoint(cluster[point]));
            validCluster.Add(FIntPoint(cluster[point].X, cluster[point].Y + 1));
            validCluster.Add(FIntPoint(cluster[point].X, cluster[point].Y + 2));
            validCluster.Add(FIntPoint(cluster[point].X, cluster[point].Y + 3));

            return validCluster;
        }

        //vertical
        if (cluster.Contains(FIntPoint(cluster[point].X + 1, cluster[point].Y)) && cluster.Contains(FIntPoint(cluster[point].X + 2, cluster[point].Y)) && cluster.Contains(FIntPoint(cluster[point].X + 3, cluster[point].Y))) {
            validCluster.Add(FIntPoint(cluster[point]));
            validCluster.Add(FIntPoint(cluster[point].X + 1, cluster[point].Y));
            validCluster.Add(FIntPoint(cluster[point].X + 2, cluster[point].Y));
            validCluster.Add(FIntPoint(cluster[point].X + 3, cluster[point].Y));

            return validCluster;
        }

        //2x2 Matrix
        if (cluster.Contains(FIntPoint(cluster[point].X, cluster[point].Y + 1)) && cluster.Contains(FIntPoint(cluster[point].X + 1, cluster[point].Y)) && cluster.Contains(FIntPoint(cluster[point].X + 1, cluster[point].Y + 1))) {
            validCluster.Add(FIntPoint(cluster[point]));
            validCluster.Add(FIntPoint(cluster[point].X, cluster[point].Y + 1));
            validCluster.Add(FIntPoint(cluster[point].X + 1, cluster[point].Y));
            validCluster.Add(FIntPoint(cluster[point].X + 1, cluster[point].Y + 1));

            return validCluster;
        }
    }

    return checkSize3(cluster);
}

TArray<FIntPoint> AGridManager::checkSize5(TArray<FIntPoint>& cluster)
{
    TArray<FIntPoint> validCluster;
    TMap<int32, int32> mapX, mapY;
    for (auto point : cluster) {

        //horizontal
        if (cluster.Contains(FIntPoint(point.X, point.Y + 1)) && cluster.Contains(FIntPoint(point.X, point.Y + 2)) && cluster.Contains(FIntPoint(point.X, point.Y + 3)) && cluster.Contains(FIntPoint(point.X, point.Y + 4))) {
            validCluster.Add(FIntPoint(point));
            validCluster.Add(FIntPoint(point.X, point.Y + 1));
            validCluster.Add(FIntPoint(point.X, point.Y + 2));
            validCluster.Add(FIntPoint(point.X, point.Y + 3));
            validCluster.Add(FIntPoint(point.X, point.Y + 4));

            return validCluster;
        }

        //vertical
        if (cluster.Contains(FIntPoint(point.X + 1, point.Y)) && cluster.Contains(FIntPoint(point.X + 2, point.Y)) && cluster.Contains(FIntPoint(point.X + 3, point.Y)) && cluster.Contains(FIntPoint(point.X + 4, point.Y))) {
            validCluster.Add(FIntPoint(point));
            validCluster.Add(FIntPoint(point.X + 1, point.Y));
            validCluster.Add(FIntPoint(point.X + 2, point.Y));
            validCluster.Add(FIntPoint(point.X + 3, point.Y));
            validCluster.Add(FIntPoint(point.X + 4, point.Y));

            return validCluster;
        }
        // L or T Shape
        mapX.FindOrAdd(point.X)++;
        mapY.FindOrAdd(point.Y)++;

        //mapX.ValueSort();
        int32 maxCountX = 0, maxCountY = 0, maxX = -1, maxY = -1;

        for (const TPair<int32, int32>& Pair : mapX) {
            if (Pair.Value > maxCountX) {
                maxX = Pair.Key;
                maxCountX = Pair.Value;
            }
        }
        for (const TPair<int32, int32>& Pair : mapY) {
            if (Pair.Value > maxCountY) {
                maxY = Pair.Key;
                maxCountY = Pair.Value;
            }
        }

        if (maxCountX == 3 && maxCountY == 3 && cluster.Num() == 5) {
            return cluster;
        }
        else if (maxCountX >= 3 && maxCountY >= 3) {
            UE_LOG(LogTemp, Warning, TEXT("Mid Tile: %d,%d"), maxX, maxY);
            //checkForSize6(validCluster,cluster,maxX,maxY);
            validCluster.Add(FIntPoint(maxX, maxY));

            //upper L
            if (cluster.Contains(FIntPoint(maxX - 1, maxY)) && cluster.Contains(FIntPoint(maxX - 2, maxY))) {
                validCluster.Add(FIntPoint(maxX - 1, maxY)); validCluster.Add(FIntPoint(maxX - 2, maxY));

                //  upper left L
                if (cluster.Contains(FIntPoint(maxX, maxY - 1)) && cluster.Contains(FIntPoint(maxX, maxY - 2))) {
                    validCluster.Add(FIntPoint(maxX, maxY - 1)); validCluster.Add(FIntPoint(maxX, maxY - 2));
                    return validCluster;
                }
                // upper right L
                else if (cluster.Contains(FIntPoint(maxX, maxY + 1)) && cluster.Contains(FIntPoint(maxX, maxY + 2))) {
                    validCluster.Add(FIntPoint(maxX, maxY + 1)); validCluster.Add(FIntPoint(maxX, maxY + 2));
                    return validCluster;
                }

                //upper T
                else if (cluster.Contains(FIntPoint(maxX, maxY - 1)) && cluster.Contains(FIntPoint(maxX, maxY + 1))) {
                    validCluster.Add(FIntPoint(maxX, maxY - 1)); validCluster.Add(FIntPoint(maxX, maxY + 1));
                    return validCluster;
                }
            }

            //lower L
            else if (cluster.Contains(FIntPoint(maxX + 1, maxY)) && cluster.Contains(FIntPoint(maxX + 2, maxY))) {
                validCluster.Add(FIntPoint(maxX + 1, maxY)); validCluster.Add(FIntPoint(maxX + 2, maxY));
                //  lower left L
                if (cluster.Contains(FIntPoint(maxX, maxY - 1)) && cluster.Contains(FIntPoint(maxX, maxY - 2))) {
                    validCluster.Add(FIntPoint(maxX, maxY - 1)); validCluster.Add(FIntPoint(maxX, maxY - 2));
                    return validCluster;
                }
                // lower right L
                else if (cluster.Contains(FIntPoint(maxX, maxY + 1)) && cluster.Contains(FIntPoint(maxX, maxY + 2))) {
                    validCluster.Add(FIntPoint(maxX, maxY + 1)); validCluster.Add(FIntPoint(maxX, maxY + 2));
                    return validCluster;
                }
                //Lower T
                else if (cluster.Contains(FIntPoint(maxX, maxY - 1)) && cluster.Contains(FIntPoint(maxX, maxY + 1))) {
                    validCluster.Add(FIntPoint(maxX, maxY - 1)); validCluster.Add(FIntPoint(maxX, maxY + 1));
                    return validCluster;
                }
            }

            //left T
            else if (cluster.Contains(FIntPoint(maxX, maxY - 1)) && cluster.Contains(FIntPoint(maxX, maxY - 2))) {
                validCluster.Add(FIntPoint(maxX, maxY - 1)); validCluster.Add(FIntPoint(maxX, maxY - 2));
                if (cluster.Contains(FIntPoint(maxX - 1, maxY)) && cluster.Contains(FIntPoint(maxX + 1, maxY))) {
                    validCluster.Add(FIntPoint(maxX - 1, maxY)); validCluster.Add(FIntPoint(maxX + 1, maxY));
                    return validCluster;
                }
            }

            //right T
            else if (cluster.Contains(FIntPoint(maxX, maxY + 1)) && cluster.Contains(FIntPoint(maxX, maxY + 2))) {
                validCluster.Add(FIntPoint(maxX, maxY + 1)); validCluster.Add(FIntPoint(maxX, maxY + 2));

                if (cluster.Contains(FIntPoint(maxX - 1, maxY)) && cluster.Contains(FIntPoint(maxX + 1, maxY))) {
                    validCluster.Add(FIntPoint(maxX - 1, maxY)); validCluster.Add(FIntPoint(maxX + 1, maxY));
                    return validCluster;
                }
            }
        }
    }


    return checkSize4(cluster);


    return checkSize4(cluster);
}

void AGridManager::SpecialGemSpawn(TArray<FIntPoint>& cluster)
{
    //TODO
    if (cluster.Num() == 3)
        return;

    //if cluster formed not by move but cascading tiles
    if (LastMove.IsUnset())
    {
        FIntPoint point = cluster[0];
        TileGrid[point.X][point.Y] = GetWorld()->SpawnActor<ATile>(TileGemClass, FVector((point.X) * 140.0f, (point.Y) * 140.0f, 0), FRotator(0.0, 90.0, 90.0));
        TileGrid[point.X][point.Y]->SetRowCol(point.X, point.Y);

        if (cluster.Num() == 4)
        {
            TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Watermelon);
        }
        else if (cluster.Num() == 5)
        {
            
            if(cluster[0].Y == cluster[1].Y && cluster[1].Y == cluster[2].Y && cluster[2].Y == cluster[3].Y && cluster[3].Y == cluster[4].Y)
                TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Sugarcane_Horizontal); 
            else
                TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Sugarcane_Vertical);
        }
        else
        {
            TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Pineapple);
        }
    }
    else
    {
        for (FIntPoint point : cluster)
        {
            if (point == LastMove.TileLocationA || point == LastMove.TileLocationB)
            {
                TileGrid[point.X][point.Y] = GetWorld()->SpawnActor<ATile>(TileGemClass, FVector((point.X) * 140.0f, (point.Y) * 140.0f, 0), FRotator(0.0, 90.0, 90.0));
                TileGrid[point.X][point.Y]->SetRowCol(point.X, point.Y);

                if (cluster.Num() == 4)
                {
                    TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Watermelon);
                }
                else if (cluster.Num() == 5)
                {
                    if (cluster[0].Y == cluster[1].Y && cluster[1].Y == cluster[2].Y && cluster[2].Y == cluster[3].Y && cluster[3].Y == cluster[4].Y)
                        TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Sugarcane_Horizontal);
                    else
                        TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Sugarcane_Vertical);
                }
                else
                {
                    TileGrid[point.X][point.Y]->SetSpecificType(ESpecificType::Pineapple);
                }

                break;

            }


        }
    }
}

void AGridManager::MoveTilesDown()
{
    for (int row = Rows - 1; row >= 0; row--)
    {
        for (int col = Columns - 1; col >= 0; col--)
        {
            if (TileGrid[row][col] == nullptr )
            {
                int upperRow = row - 1;
                while (upperRow >= 0 && TileGrid[upperRow][col] == nullptr)
                {
                    upperRow -= 1;
                }

                if (upperRow >= 0)
                {
                    //swap with above
                    ATile* swapTileObj = TileGrid[upperRow][col];

                    TileGrid[upperRow][col] = nullptr;

                    swapTileObj->rowIndex = row;
                    swapTileObj->colIndex = col;

                    TileGrid[row][col] = swapTileObj;

                    swapTileObj->moveDownAnimate(row - upperRow);

                }
                else
                {
                    //no tile/sprite above
                    int offset = row + 2;//Rows + (Rows - 1) - row;
                    TileGrid[row][col] = GetWorld()->SpawnActor<ATile>(TileFruitClass, FVector((row-offset) * 140.0f, col * 140.0f, 0),FRotator(0.0, 90.0, 90.0));
                    TileGrid[row][col]->SetRowCol(row, col);

                    int randFruit = rand() % 4;
                    switch (randFruit)
                    {
                    case 0: TileGrid[row][col]->SetSpecificType(ESpecificType::Apple);
                        break;
                    case 1: TileGrid[row][col]->SetSpecificType(ESpecificType::Banana);
                        break;
                    case 2: TileGrid[row][col]->SetSpecificType(ESpecificType::Orange);
                        break;
                    case 3: TileGrid[row][col]->SetSpecificType(ESpecificType::Grape);
                        break;

                    }
                    
                    TileGrid[row][col]->moveDownAnimate(offset);

                }

            }
        }
    }
    GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::CheckForClusters, 0.5f, false);
}

void AGridManager::SpecialGemBurstAffect(FIntPoint GemTileLoc, TArray<FIntPoint> burstAffectedTiles)
{
    // Trigger burst animation on the main gem tile
    
    //remove sprite of gem tile
    ATile* GemTile = TileGrid[GemTileLoc.X][GemTileLoc.Y];
    GemTile->SpriteComponent->SetSprite(nullptr);

    ATile* AnimatingTile;
    if (GemTile->SpecificType == ESpecificType::Sugarcane_Vertical)
    {
        AnimatingTile = TileGrid[Rows/2][GemTileLoc.Y];
        AnimatingTile->BurstAnimate(EBurstAnimationType::Sugarcane_Vertical_Explosion);
    }
    else if (GemTile->SpecificType == ESpecificType::Sugarcane_Horizontal)
    {
        AnimatingTile = TileGrid[GemTileLoc.X][Columns/2];
        AnimatingTile->BurstAnimate(EBurstAnimationType::Sugarcane_Horizontal_Explosion);
    }
    else if (GemTile->SpecificType == ESpecificType::Watermelon)
    {
        AnimatingTile = GemTile;
        AnimatingTile->BurstAnimate(EBurstAnimationType::Watermelon_Explosion);        
    }
    else
    {
        AnimatingTile = GemTile;
        AnimatingTile->BurstAnimate(EBurstAnimationType::Pineapple_Explosion);
    }
    //remove sprite of AnimatingTile
    if(AnimatingTile!=nullptr)
        AnimatingTile->SpriteComponent->SetSprite(nullptr);

    // Iterate over each affected tile
    for (auto& tileIndex : burstAffectedTiles)
    {
        if (TileGrid[tileIndex.X][tileIndex.Y] == nullptr)
            continue;

        if (TileGrid[tileIndex.X][tileIndex.Y]->TileType == ETileType::Gem)
        {
            ATileGem* AffectedGem = Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y]);
            if (AffectedGem && !AffectedGem->isBursted)
            {
                ATileGem* TileGem = Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y]);
                if (TileGem)
                {
                    TileGem->GemBurstLogic(FIntPoint(-1, -1));  // Trigger burst logic and animation
                }
            }
        }
        else
        {
            if (TileGrid[tileIndex.X][tileIndex.Y] 
                && TileGrid[tileIndex.X][tileIndex.Y] != AnimatingTile
                )
            {
                TileGrid[tileIndex.X][tileIndex.Y]->Destroy();
                TileGrid[tileIndex.X][tileIndex.Y] = nullptr;
            }
            else if (TileGrid[tileIndex.X][tileIndex.Y] && TileGrid[tileIndex.X][tileIndex.Y] == AnimatingTile)
            {
                TileGrid[tileIndex.X][tileIndex.Y] = nullptr;
            }
        }

    }
    //TODO: Mark to Destroy these later

    if (TileGrid[GemTileLoc.X][GemTileLoc.Y])
    {
        //TileGrid[GemTileLoc.X][GemTileLoc.Y]->Destroy();
        TileGrid[GemTileLoc.X][GemTileLoc.Y] = nullptr;
    }

}

void AGridManager::OnTileClickReceived(int tileRowIndex, int tileColIndex)
{
    TileGrid[tileRowIndex][tileColIndex]->HandleTileClick();
    //
    GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], this, &AGridManager::MoveTilesDown, 0.5f, false);
}

// Initialize the static instance pointer to nullptr
AGridManager* AGridManager::Instance = nullptr;


ATile* AGridManager::GetTileAt(int tileRowIndex, int tileColIndex) const
{
    return TileGrid[tileRowIndex][tileColIndex];
}

AGridManager* AGridManager::GetInstance()
{
    return Instance;
}

void AGridManager::SetInstance(AGridManager* InGridManager)
{
    //if (!Instance)
    //{
    Instance = InGridManager;
    //}
}
















////TODO need to take care that affected tile being special gem while destroying cause problem(if swapped with special gem)
    //Cast<ATileGem>(TileGrid[GemTileLoc.X][GemTileLoc.Y])->GemBurstAnimate();
    //
    //for (auto tileIndex : burstAffectedTiles)
    //{
    //    if (TileGrid[tileIndex.X][tileIndex.Y] == nullptr)
    //        continue;

    //    else if (TileGrid[tileIndex.X][tileIndex.Y]->TileType == ETileType::Gem && Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y])->isBursted == false)
    //        Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y])->GemBurstLogic(FIntPoint(-1, -1));

    //    else
    //    {
    //        TileGrid[tileIndex.X][tileIndex.Y]->Destroy();
    //        TileGrid[tileIndex.X][tileIndex.Y] = nullptr;
    //    }
    //    

    //}

    ////delete special tile also after delay
    //GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, GemTileLoc,burstAffectedTiles]{
    //    
    //    if (TileGrid[GemTileLoc.X][GemTileLoc.Y])
    //    {
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y]->Destroy();
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y] = nullptr;
    //    }
    //        
    //}, 0.25f, false);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    //// Animate the main gem tile
    //Cast<ATileGem>(TileGrid[GemTileLoc.X][GemTileLoc.Y])->GemBurstAnimate();

    //// Iterate over each affected tile and apply a delayed burst animation and destruction
    //int32 DelayIndex = 0;
    //for (auto& tileIndex : burstAffectedTiles)
    //{
    //    if (TileGrid[tileIndex.X][tileIndex.Y] == nullptr)
    //        continue;

    //    if (TileGrid[tileIndex.X][tileIndex.Y]->TileType == ETileType::Gem && Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y])->isBursted == false)
    //    {
    //        // Delay each gem burst effect by increasing amounts
    //        GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, tileIndex]() {
    //            Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y])->GemBurstLogic(FIntPoint(-1, -1));
    //            }, 0.015f * DelayIndex, false);  // Adjust the delay multiplier (e.g., 0.1f) as needed
    //    }
    //    else
    //    {
    //        // Destroy non-gem tiles with a delay to allow animation to complete
    //        //GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, tileIndex]() {
    //            TileGrid[tileIndex.X][tileIndex.Y]->Destroy();
    //            TileGrid[tileIndex.X][tileIndex.Y] = nullptr;
    //            //}, 0.1f * DelayIndex, false);
    //    }

    //    DelayIndex++;  // Increase delay for each tile
    //}

    //// Finalize by destroying the original special gem tile with a slight delay after all animations
    //GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, GemTileLoc]() {
    //    if (TileGrid[GemTileLoc.X][GemTileLoc.Y])
    //    {
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y]->Destroy();
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y] = nullptr;
    //    }
    //}, 0.20f /*+ 0.1f * DelayIndex*/, false);  // Add final delay after other effects

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //// Trigger burst animation on the main gem tile
    //ATileGem * MainGemTile = Cast<ATileGem>(TileGrid[GemTileLoc.X][GemTileLoc.Y]);
    //if (MainGemTile)
    //{
    //    MainGemTile->GemBurstAnimate();
    //}

    //int32 DelayIndex = 1;  // Start with delay multiplier at 1 for clarity

    //// Iterate over each affected tile
    //for (auto& tileIndex : burstAffectedTiles)
    //{
    //    if (TileGrid[tileIndex.X][tileIndex.Y] == nullptr)
    //        continue;

    //    if (TileGrid[tileIndex.X][tileIndex.Y]->TileType == ETileType::Gem)
    //    {
    //        ATileGem* AffectedGem = Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y]);
    //        if (AffectedGem && !AffectedGem->isBursted)
    //        {
    //            // Delay gem burst logic and animation, ensuring each plays sequentially
    //            GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, tileIndex]() {
    //                ATileGem* TileGem = Cast<ATileGem>(TileGrid[tileIndex.X][tileIndex.Y]);
    //                if (TileGem)
    //                {
    //                    TileGem->GemBurstLogic(FIntPoint(-1, -1));  // Trigger burst logic and animation
    //                }
    //                }, 0.02f * DelayIndex, false);  // Delay each gem burst by 0.2s increments
    //        }
    //    }
    //    else
    //    {
    //        // Destroy non-gem tiles with a delay to allow animation to complete
    //        //GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, tileIndex]() {
    //            if (TileGrid[tileIndex.X][tileIndex.Y] && TileGrid[tileIndex.X][tileIndex.Y]!=MainGemTile)
    //            {
    //                TileGrid[tileIndex.X][tileIndex.Y]->Destroy();
    //                TileGrid[tileIndex.X][tileIndex.Y] = nullptr;
    //            }
    //            //}, 0.2f * DelayIndex, false);  // Adjust delay multiplier (e.g., 0.2f)
    //    }

    //    DelayIndex++;  // Increase delay for each tile
    //}

    //// Finalize by destroying the main special gem tile after all animations
    //GetWorld()->GetTimerManager().SetTimer(timerHandler[(timerHandlerOffset++) % 10], [this, GemTileLoc]() {
    //    if (TileGrid[GemTileLoc.X][GemTileLoc.Y])
    //    {
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y]->Destroy();
    //        TileGrid[GemTileLoc.X][GemTileLoc.Y] = nullptr;
    //    }
    //    }, 0.03f * (DelayIndex + 1), false);  // Ensure main gem destruction occurs after all animations


