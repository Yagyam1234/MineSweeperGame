// MinesweeperToolCommands.cpp
#include "MinesweeperToolCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperToolModule"

void FMinesweeperToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Minesweeper", "Open the Minesweeper game", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE