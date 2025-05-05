// MinesweeperGame.h
#pragma once

#include "CoreMinimal.h"

class FMinesweeperGame
{
public:
	enum class ETileState
	{
		Hidden,
		Revealed,
		Exploded
	};

	struct FTile
	{
		bool bIsBomb = false;
		int32 AdjacentBombs = 0;
		ETileState State = ETileState::Hidden;
	};

	FMinesweeperGame();

	// Initialize a new game
	void NewGame(int32 InWidth, int32 InHeight, int32 InBombCount);
    
	// Reveal a tile at the given coordinates
	bool RevealTile(int32 X, int32 Y);
    
	// Check if coordinate is valid
	bool IsValidCoordinate(int32 X, int32 Y) const;
    
	// Get tile at position
	const FTile& GetTile(int32 X, int32 Y) const;
    
	// Game state
	bool IsGameOver() const { return bGameOver; }
	bool IsGameWon() const { return bGameWon; }
    
	// Grid properties
	int32 GetWidth() const { return Width; }
	int32 GetHeight() const { return Height; }
	int32 GetBombCount() const { return BombCount; }

private:
	// Place bombs randomly on the grid
	void PlaceBombsRandomly(int32 SafeX, int32 SafeY);
    
	// Calculate adjacent bomb counts for all tiles
	void CalculateAdjacentBombs();
    
	// Recursively reveal empty tiles
	void FloodFillReveal(int32 X, int32 Y);
    
	// Check if the game is won
	void CheckGameWon();

	TArray<FTile> Grid;
	int32 Width;
	int32 Height;
	int32 BombCount;
	bool bGameOver;
	bool bGameWon;
	int32 RevealedTiles;
};