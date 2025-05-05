// SMinesweeperWindow.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSpinBox.h"
#include "MinesweeperGame.h"


class SButton;
class SGridPanel;
class STextBlock;

class SMinesweeperWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWindow)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// Game state
	TSharedPtr<FMinesweeperGame> Game;
    
	// UI References
	TSharedPtr<SSpinBox<int32>> WidthSpinBox;
	TSharedPtr<SSpinBox<int32>> HeightSpinBox;
	TSharedPtr<SSpinBox<int32>> BombCountSpinBox;
	TSharedPtr<SButton> NewGameButton;
	TSharedPtr<SGridPanel> GameGrid;
	TSharedPtr<STextBlock> GameStatusText;
    
	// Event handlers
	FReply OnNewGameClicked();
	FReply OnTileClicked(int32 X, int32 Y);
    
	// UI builders
	TSharedRef<SWidget> BuildConfigPanel();
	TSharedRef<SWidget> BuildGameGrid();
	void UpdateGameGrid();
	void UpdateGameStatus();
    
	// Utility
	void ValidateBombCount(int32 NewValue);
};