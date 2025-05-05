// MinesweeperGame.cpp
#include "MinesweeperGame.h"
#include "Math/UnrealMathUtility.h"

FMinesweeperGame::FMinesweeperGame()
    : Width(0)
    , Height(0)
    , BombCount(0)
    , bGameOver(false)
    , bGameWon(false)
    , RevealedTiles(0)
{
}

void FMinesweeperGame::NewGame(int32 InWidth, int32 InHeight, int32 InBombCount)
{
    // Validate input parameters
    Width = FMath::Max(1, InWidth);
    Height = FMath::Max(1, InHeight);
    
    // Ensure there's at least one safe tile
    const int32 MaxBombs = (Width * Height) - 1;
    BombCount = FMath::Clamp(InBombCount, 0, MaxBombs);
    
    // Reset game state
    bGameOver = false;
    bGameWon = false;
    RevealedTiles = 0;
    
    // Initialize grid
    Grid.Empty(Width * Height);
    Grid.AddDefaulted(Width * Height);
    
    // Don't place bombs yet - we'll do that on first click to ensure
    // the first click is never a bomb
}

bool FMinesweeperGame::RevealTile(int32 X, int32 Y)
{
    if (!IsValidCoordinate(X, Y))
    {
        return false;
    }
    
    FTile& Tile = Grid[Y * Width + X];
    
    // If tile is already revealed, do nothing
    if (Tile.State != ETileState::Hidden)
    {
        return false;
    }
    
    // First click - initialize bombs ensuring this tile is safe
    if (RevealedTiles == 0)
    {
        PlaceBombsRandomly(X, Y);
        CalculateAdjacentBombs();
    }
    
    // Reveal the tile
    if (Tile.bIsBomb)
    {
        // Game over
        Tile.State = ETileState::Exploded;
        bGameOver = true;
        
        // Also reveal all remaining bombs
        for (FTile& OtherTile : Grid)
        {
            if (OtherTile.bIsBomb && OtherTile.State == ETileState::Hidden)
            {
                OtherTile.State = ETileState::Revealed;
            }
        }
        
        return true;
    }
    
    // Reveal this tile
    Tile.State = ETileState::Revealed;
    RevealedTiles++;
    
    // If this is an empty tile, reveal surrounding tiles
    if (Tile.AdjacentBombs == 0)
    {
        FloodFillReveal(X, Y);
    }
    
    // Check if game is won
    CheckGameWon();
    
    return true;
}

bool FMinesweeperGame::IsValidCoordinate(int32 X, int32 Y) const
{
    return X >= 0 && X < Width && Y >= 0 && Y < Height;
}

const FMinesweeperGame::FTile& FMinesweeperGame::GetTile(int32 X, int32 Y) const
{
    if (IsValidCoordinate(X, Y))
    {
        return Grid[Y * Width + X];
    }
    
    static FTile InvalidTile;
    return InvalidTile;
}

void FMinesweeperGame::PlaceBombsRandomly(int32 SafeX, int32 SafeY)
{
    const int32 SafeIndex = SafeY * Width + SafeX;
    
    // Create a list of valid tile indices (all tiles except the first click)
    TArray<int32> ValidTileIndices;
    ValidTileIndices.Reserve(Width * Height - 1);
    
    for (int32 i = 0; i < Width * Height; ++i)
    {
        if (i != SafeIndex)
        {
            ValidTileIndices.Add(i);
        }
    }
    
    // Place bombs randomly
    int32 BombsToPlace = BombCount;
    while (BombsToPlace > 0 && ValidTileIndices.Num() > 0)
    {
        // Pick a random index from the valid tiles
        const int32 RandomIndex = FMath::RandRange(0, ValidTileIndices.Num() - 1);
        const int32 TileIndex = ValidTileIndices[RandomIndex];
        
        // Remove this index from the valid tiles
        ValidTileIndices.RemoveAt(RandomIndex);
        
        // Set this tile as a bomb
        Grid[TileIndex].bIsBomb = true;
        BombsToPlace--;
    }
}

void FMinesweeperGame::CalculateAdjacentBombs()
{
    // For each tile, count adjacent bombs
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            if (!Grid[Y * Width + X].bIsBomb)
            {
                int32 AdjacentBombs = 0;
                
                // Check all 8 surrounding tiles
                for (int32 DY = -1; DY <= 1; ++DY)
                {
                    for (int32 DX = -1; DX <= 1; ++DX)
                    {
                        // Skip the current tile
                        if (DX == 0 && DY == 0)
                        {
                            continue;
                        }
                        
                        int32 CheckX = X + DX;
                        int32 CheckY = Y + DY;
                        
                        if (IsValidCoordinate(CheckX, CheckY) && Grid[CheckY * Width + CheckX].bIsBomb)
                        {
                            AdjacentBombs++;
                        }
                    }
                }
                
                Grid[Y * Width + X].AdjacentBombs = AdjacentBombs;
            }
        }
    }
}

void FMinesweeperGame::FloodFillReveal(int32 X, int32 Y)
{
    // Basic BFS to reveal empty tiles and their adjacent numbered tiles
    TArray<TPair<int32, int32>> Queue;
    Queue.Add(TPair<int32, int32>(X, Y));
    
    while (Queue.Num() > 0)
    {
        TPair<int32, int32> Current = Queue[0];
        Queue.RemoveAt(0);
        
        int32 CurrentX = Current.Key;
        int32 CurrentY = Current.Value;
        
        // Check all 8 surrounding tiles
        for (int32 DY = -1; DY <= 1; ++DY)
        {
            for (int32 DX = -1; DX <= 1; ++DX)
            {
                int32 CheckX = CurrentX + DX;
                int32 CheckY = CurrentY + DY;
                
                if (IsValidCoordinate(CheckX, CheckY))
                {
                    FTile& CheckTile = Grid[CheckY * Width + CheckX];
                    
                    // Only process hidden tiles
                    if (CheckTile.State == ETileState::Hidden)
                    {
                        // Don't reveal bombs
                        if (!CheckTile.bIsBomb)
                        {
                            CheckTile.State = ETileState::Revealed;
                            RevealedTiles++;
                            
                            // If this is also an empty tile, add it to the queue
                            if (CheckTile.AdjacentBombs == 0)
                            {
                                Queue.Add(TPair<int32, int32>(CheckX, CheckY));
                            }
                        }
                    }
                }
            }
        }
    }
}

void FMinesweeperGame::CheckGameWon()
{
    // Game is won when all non-bomb tiles are revealed
    if (RevealedTiles == (Width * Height - BombCount))
    {
        bGameWon = true;
    }
}