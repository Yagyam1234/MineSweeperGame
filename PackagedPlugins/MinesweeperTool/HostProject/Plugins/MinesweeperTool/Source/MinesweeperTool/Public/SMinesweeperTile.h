// SMinesweeperTile.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "MinesweeperGame.h"

class SMinesweeperTile : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperTile)
		: _X(0)
		, _Y(0)
		, _Game(nullptr)
		, _OnTileClicked()
	{}
	SLATE_ARGUMENT(int32, X)
	SLATE_ARGUMENT(int32, Y)
	SLATE_ARGUMENT(TSharedPtr<FMinesweeperGame>, Game)
	SLATE_EVENT(FOnClicked, OnTileClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

	// SWidget interface
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
						const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, 
						int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// End of SWidget interface

	private:
	int32 X;
	int32 Y;
	TSharedPtr<FMinesweeperGame> Game;
	FOnClicked OnTileClicked;
    
	FSlateColor GetTileColor() const;
	FText GetTileText() const;
};