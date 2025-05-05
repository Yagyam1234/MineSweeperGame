// SMinesweeperWindow.cpp
#include "SMinesweeperWindow.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#include "SMinesweeperTile.h"

#define LOCTEXT_NAMESPACE "MinesweeperTool"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperWindow::Construct(const FArguments& InArgs)
{
    // Initialize the game
    Game = MakeShared<FMinesweeperGame>();
    
    // Create the window content
    ChildSlot
    [
        SNew(SVerticalBox)
        
        // Title
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("WindowTitle", "Minesweeper"))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
        ]
        
        // Configuration panel
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10)
        [
            BuildConfigPanel()
        ]
        
        // Status text
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10)
        [
            SAssignNew(GameStatusText, STextBlock)
            .Text(LOCTEXT("ReadyStatus", "Ready to play!"))
            .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
        ]
        
        // Game grid in a scroll box
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .Padding(10)
        [
            SNew(SScrollBox)
            .Orientation(Orient_Vertical)
            + SScrollBox::Slot()
            [
                SNew(SBox)
                .MinDesiredWidth(300)
                .MinDesiredHeight(300)
                [
                    SAssignNew(GameGrid, SGridPanel)
                ]
            ]
        ]
    ];
    
    // Start with a default game (10x10 with 10 bombs)
    WidthSpinBox->SetValue(10);
    HeightSpinBox->SetValue(10);
    BombCountSpinBox->SetValue(10);
    OnNewGameClicked();
}

FReply SMinesweeperWindow::OnNewGameClicked()
{
    // Get config values
    int32 Width = WidthSpinBox->GetValue();
    int32 Height = HeightSpinBox->GetValue();
    int32 BombCount = BombCountSpinBox->GetValue();
    
    // Initialize new game
    Game->NewGame(Width, Height, BombCount);
    
    // Update UI
    UpdateGameGrid();
    UpdateGameStatus();
    
    return FReply::Handled();
}

FReply SMinesweeperWindow::OnTileClicked(int32 X, int32 Y)
{
    // Ignore clicks if game is over
    if (Game->IsGameOver() || Game->IsGameWon())
    {
        return FReply::Handled();
    }
    
    // Process the click
    Game->RevealTile(X, Y);
    
    // Update status
    UpdateGameStatus();
    
    return FReply::Handled();
}

TSharedRef<SWidget> SMinesweeperWindow::BuildConfigPanel()
{
    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
        .Padding(4.0f)
        [
            SNew(SHorizontalBox)
            
            // Width
            + SHorizontalBox::Slot()
            .Padding(4, 0)
            .AutoWidth()
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("WidthLabel", "Width"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(WidthSpinBox, SSpinBox<int32>)
                    .MinValue(5)
                    .MaxValue(50)
                    .Delta(1)
                    .OnValueChanged(this, &SMinesweeperWindow::ValidateBombCount)
                ]
            ]
            
            // Height
            + SHorizontalBox::Slot()
            .Padding(4, 0)
            .AutoWidth()
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("HeightLabel", "Height"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(HeightSpinBox, SSpinBox<int32>)
                    .MinValue(5)
                    .MaxValue(50)
                    .Delta(1)
                    .OnValueChanged(this, &SMinesweeperWindow::ValidateBombCount)
                ]
            ]
            
            // Bomb Count
            + SHorizontalBox::Slot()
            .Padding(4, 0)
            .AutoWidth()
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("BombCountLabel", "Bombs"))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SAssignNew(BombCountSpinBox, SSpinBox<int32>)
                    .MinValue(1)
                    .MaxValue(100) // This will be constrained by ValidateBombCount
                    .Delta(1)
                ]
            ]
            
            // Spacer
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            
            // New Game Button
            + SHorizontalBox::Slot()
            .Padding(4, 0)
            .AutoWidth()
            .VAlign(VAlign_Bottom)
            [
                SAssignNew(NewGameButton, SButton)
                .Text(LOCTEXT("NewGameButton", "New Game"))
                .OnClicked(this, &SMinesweeperWindow::OnNewGameClicked)
            ]
        ];
}

void SMinesweeperWindow::UpdateGameGrid()
{
    // Clear existing grid
    GameGrid->ClearChildren();
    
    // Get game dimensions
    const int32 Width = Game->GetWidth();
    const int32 Height = Game->GetHeight();
    
    // Create a grid of tile widgets
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            GameGrid->AddSlot(X, Y)
            .Padding(1)
            [
                SNew(SMinesweeperTile)
                .X(X)
                .Y(Y)
                .Game(Game)
                .OnTileClicked(this, &SMinesweeperWindow::OnTileClicked, X, Y)
            ];
        }
    }
}

void SMinesweeperWindow::UpdateGameStatus()
{
    if (Game->IsGameOver())
    {
        GameStatusText->SetText(LOCTEXT("GameOverStatus", "Game Over! Try again?"));
        GameStatusText->SetColorAndOpacity(FLinearColor::Red);
    }
    else if (Game->IsGameWon())
    {
        GameStatusText->SetText(LOCTEXT("GameWonStatus", "You Win! Congratulations!"));
        GameStatusText->SetColorAndOpacity(FLinearColor::Green);
    }
    else
    {
        GameStatusText->SetText(LOCTEXT("PlayingStatus", "Playing..."));
        GameStatusText->SetColorAndOpacity(FLinearColor::White);
    }
}

void SMinesweeperWindow::ValidateBombCount(int32 NewValue)
{
    // Make sure bomb count doesn't exceed (width * height - 1)
    int32 Width = WidthSpinBox->GetValue();
    int32 Height = HeightSpinBox->GetValue();
    int32 MaxBombs = (Width * Height) - 1;
    
    // Update max value for bomb count spinner
    BombCountSpinBox->SetMaxValue(MaxBombs);
    
    // Clamp current value if needed
    int32 CurrentBombs = BombCountSpinBox->GetValue();
    if (CurrentBombs > MaxBombs)
    {
        BombCountSpinBox->SetValue(MaxBombs);
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE