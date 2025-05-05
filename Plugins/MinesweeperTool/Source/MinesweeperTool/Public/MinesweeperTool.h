// MinesweeperToolModule.h
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

class FToolBarBuilder;
class FMenuBuilder;
class SDockTab;

class FMinesweeperToolModule : public IModuleInterface
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
	// Callback for when the minesweeper button is clicked
	void PluginButtonClicked();
    
private:
	// Register and create the plugin UI
	void RegisterMenus();
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
	void OnPluginTabClosed(TSharedRef<SDockTab> TabClosed);
    
	// Plugin commands
	TSharedPtr<class FUICommandList> PluginCommands;
    
	// Keep track of the opened tab
	TSharedPtr<SDockTab> MinesweeperTab;
};