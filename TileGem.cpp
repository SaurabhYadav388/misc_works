// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGem.h"
#include "GridManager.h"

ATileGem::ATileGem()
{
	TileType = ETileType::Gem;
	isBursted = false;
}

void ATileGem::HandleTileClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle tile click Gem: will trigger burst logic"));
	GemBurstLogic(FIntPoint(-1, -1));

}

void ATileGem::OnSwipedWith(FIntPoint OtherTileLoc)
{
	GemBurstLogic(OtherTileLoc);
}

void ATileGem::GemBurstLogic(FIntPoint OtherTileLoc)
{
	UE_LOG(LogTemp, Warning, TEXT("triggered Gem burst logic"));
	//other tile loc will help in behaviour of some special gem burst logic
	
	//if othertileloc == -1,-1
	//clicked logic

	//else swapped with  some other tile

	isBursted = true;

	TArray<FIntPoint> burstAffectedTiles;

	if (OtherTileLoc != FIntPoint(-1, -1) && AGridManager::GetInstance()->GetTileAt(OtherTileLoc.X, OtherTileLoc.Y)->TileType == ETileType::Gem)
	{
		//both are bomb
		if (SpecificType == ESpecificType::Watermelon && AGridManager::GetInstance()->GetTileAt(OtherTileLoc.X, OtherTileLoc.Y)->SpecificType == ESpecificType::Watermelon)
		{
			int startRow,endRow,startCol,endCol;
			if (rowIndex == OtherTileLoc.X)
			{
				(colIndex < OtherTileLoc.Y) ? (startCol = colIndex - 1 , endCol = OtherTileLoc.Y + 1) : (startCol = OtherTileLoc.Y - 1 , endCol = colIndex + 1);

				startRow = (rowIndex - 2);
				endRow = (rowIndex + 2);
			}
			else//same col
			{
				(rowIndex < OtherTileLoc.X) ? (startRow = rowIndex - 1 ,endRow = OtherTileLoc.X + 1) : (startRow = OtherTileLoc.X - 1, endRow = rowIndex + 1);
				startCol = colIndex - 2;
				endCol = colIndex + 2;
			}
			//UE_LOG(LogTemp, Warning, TEXT("Main   sr:%d,er:%d sc:%d,ec:%d"),startRow,endRow,startCol,endCol);
					
			//TODO : have to make sure not to send other bomb tile as its logic also get triggered
			for (int offX = startRow; offX <= endRow; offX++)
			{
				for (int offY = startCol; offY <= endCol; offY++)
				{  
					//UE_LOG(LogTemp, Warning, TEXT("1) offx:%d , offy:%d"), offX, offY);
					FIntPoint currTileLoc(offX,offY);
					if ( //!(currTileLoc.X == rowIndex && currTileLoc.Y == colIndex) && //dont send special gem tile
						currTileLoc.X >= 0 && currTileLoc.X < AGridManager::GetInstance()->Rows && currTileLoc.Y >= 0 && currTileLoc.Y < AGridManager::GetInstance()->Columns)
					{
						//UE_LOG(LogTemp, Warning, TEXT("2) offx:%d , offy:%d"), offX, offY);

						burstAffectedTiles.Add(currTileLoc);
					}
				}
			}


		}

	}
	else
	{
		if (SpecificType == ESpecificType::Watermelon)
		{
			for (int offX = -1; offX <= 1; offX++)
			{
				for (int offY = -1; offY <= 1; offY++)
				{
					FIntPoint currTileLoc(rowIndex + offX, colIndex + offY);
					if (!(currTileLoc.X == rowIndex && currTileLoc.Y == colIndex) && //dont send special gem tile
						currTileLoc.X >= 0 && currTileLoc.X < AGridManager::GetInstance()->Rows && currTileLoc.Y >= 0 && currTileLoc.Y < AGridManager::GetInstance()->Columns)
					{
						burstAffectedTiles.Add(currTileLoc);
					}
				}
			}
		}
		else if (SpecificType == ESpecificType::Sugarcane_Vertical)
		{
			int yIndex = colIndex;
			for (int ind = 0; ind < AGridManager::GetInstance()->Rows; ind++)
			{
				FIntPoint currTileLoc(ind, yIndex);
				burstAffectedTiles.Add(currTileLoc);
			}
		}
		else if (SpecificType == ESpecificType::Sugarcane_Horizontal)
		{
			int xIndex = rowIndex;
			for (int ind = 0; ind < AGridManager::GetInstance()->Columns; ind++)
			{
				FIntPoint currTileLoc(xIndex, ind);
				burstAffectedTiles.Add(currTileLoc);
			}
		}
		else if (SpecificType == ESpecificType::Pineapple)
		{
			ESpecificType TileTypeToEliminate;

			if (OtherTileLoc == FIntPoint(-1, -1))
			{
				//if clicked, pick the maximun one
				TMap<ESpecificType, int32> TileTypeCounts;

				TileTypeCounts.Add(ESpecificType::Apple, 0);
				TileTypeCounts.Add(ESpecificType::Banana, 0);
				TileTypeCounts.Add(ESpecificType::Orange, 0);
				TileTypeCounts.Add(ESpecificType::Grape, 0);

				for (int row = 0; row < AGridManager::GetInstance()->Rows; row++)
				{
					for (int col = 0; col < AGridManager::GetInstance()->Columns; col++)
					{
						if (AGridManager::GetInstance()->GetTileAt(row, col) != nullptr && AGridManager::GetInstance()->GetTileAt(row, col)->TileType == ETileType::Fruit)
						{
							ESpecificType SpecTileType = AGridManager::GetInstance()->GetTileAt(row, col)->SpecificType; // Assume each tile has a method GetTileType()
							TileTypeCounts[SpecTileType]++;
						}

					}
				}
				ESpecificType MostFrequentType = ESpecificType::Apple; // Default value
				int32 MaxCount = 0;

				for (const auto& Elem : TileTypeCounts)
				{
					if (Elem.Value > MaxCount)
					{
						MaxCount = Elem.Value;
						MostFrequentType = Elem.Key;
					}
				}

				TileTypeToEliminate = MostFrequentType;
			}
			else
			{
				TileTypeToEliminate = AGridManager::GetInstance()->GetTileAt(OtherTileLoc.X, OtherTileLoc.Y)->SpecificType;
			}
			//UE_LOG(LogTemp, Warning, TEXT("TileType to Eliminate:( %d )"),TileTypeToEliminate);
			for (int row = 0; row < AGridManager::GetInstance()->Rows; row++)
			{
				for (int col = 0; col < AGridManager::GetInstance()->Columns; col++)
				{
					if (AGridManager::GetInstance()->GetTileAt(row, col) != nullptr
						&& AGridManager::GetInstance()->GetTileAt(row, col)->SpecificType == TileTypeToEliminate)
					{
						burstAffectedTiles.Add(FIntPoint(row, col));
					}

				}
			}


		}
		else
		{
			////
		}
	}
	



	AGridManager::GetInstance()->SpecialGemBurstAffect(FIntPoint(rowIndex,colIndex), burstAffectedTiles);



}

