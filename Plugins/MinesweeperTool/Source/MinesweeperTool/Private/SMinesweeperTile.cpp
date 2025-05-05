// SMinesweeperTile.cpp
#include "SMinesweeperTile.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Brushes/SlateRoundedBoxBrush.h"

#define LOCTEXT_NAMESPACE "MinesweeperTool"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperTile::Construct(const FArguments& InArgs)
{
    X = InArgs._X;
    Y = InArgs._Y;
    Game = InArgs._Game;
    OnTileClicked = InArgs._OnTileClicked;
    
    ChildSlot
    [
        SNew(SBox)
        .WidthOverride(30.0f)
        .HeightOverride(30.0f)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("Button.Normal"))
            .Padding(FMargin(2.0f))
            [
                SNew(STextBlock)
                .Text(this, &SMinesweeperTile::GetTileText)
                .ColorAndOpacity(this, &SMinesweeperTile::GetTileColor)
                .Justification(ETextJustify::Center)
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
            ]
        ]
    ];
}

FReply SMinesweeperTile::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (OnTileClicked.IsBound())
        {
            // Immediately react to the click
            return OnTileClicked.Execute();
        }
    }
    
    return FReply::Handled();  // Changed from Unhandled to ensure we capture all clicks
}

int32 SMinesweeperTile::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
                             const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, 
                             int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
    
    const FMinesweeperGame::FTile& Tile = Game->GetTile(X, Y);
    const bool bIsRevealed = Tile.State != FMinesweeperGame::ETileState::Hidden;
    
    // Background
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        bIsRevealed 
            ? FCoreStyle::Get().GetBrush("Button.Pressed") 
            : FCoreStyle::Get().GetBrush("Button.Normal"),
        DrawEffects
    );
    
    // Call the parent OnPaint to draw child widgets
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 1, InWidgetStyle, bIsEnabled);
}

FSlateColor SMinesweeperTile::GetTileColor() const
{
    if (!Game.IsValid())
    {
        return FLinearColor::White;
    }
    
    const FMinesweeperGame::FTile& Tile = Game->GetTile(X, Y);
    
    if (Tile.State == FMinesweeperGame::ETileState::Hidden)
    {
        return FLinearColor::White;
    }
    else if (Tile.State == FMinesweeperGame::ETileState::Exploded)
    {
        return FLinearColor::Red;
    }
    else if (Tile.bIsBomb)
    {
        return FLinearColor::Black;
    }
    
    // Colors for the numbers 1-8
    switch (Tile.AdjacentBombs)
    {
        case 0: return FLinearColor::White;
        case 1: return FLinearColor::Blue;
        case 2: return FLinearColor::Green;
        case 3: return FLinearColor::Red;
        case 4: return FLinearColor(0.0f, 0.0f, 0.5f); // Dark Blue
        case 5: return FLinearColor(0.5f, 0.0f, 0.0f); // Dark Red
        case 6: return FLinearColor(0.0f, 0.5f, 0.5f); // Teal
        case 7: return FLinearColor::Black;
        case 8: return FLinearColor::Gray;
        default: return FLinearColor::White;
    }
}

FText SMinesweeperTile::GetTileText() const
{
    if (!Game.IsValid())
    {
        return FText::GetEmpty();
    }
    
    const FMinesweeperGame::FTile& Tile = Game->GetTile(X, Y);
    
    if (Tile.State == FMinesweeperGame::ETileState::Hidden)
    {
        return FText::GetEmpty();
    }
    else if (Tile.bIsBomb)
    {
        return LOCTEXT("BombText", "💣");
    }
    else if (Tile.AdjacentBombs == 0)
    {
        return FText::GetEmpty();
    }
    else
    {
        return FText::AsNumber(Tile.AdjacentBombs);
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE