// MinesweeperToolCommands.h
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperToolStyle.h"

class FMinesweeperToolCommands : public TCommands<FMinesweeperToolCommands>
{
public:
	FMinesweeperToolCommands()
		: TCommands<FMinesweeperToolCommands>(
			TEXT("MinesweeperTool"), 
			NSLOCTEXT("Contexts", "MinesweeperTool", "Minesweeper Tool"), 
			NAME_None, 
			FMinesweeperToolStyle::GetStyleSetName()
		)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

	// Command to open the plugin window
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};