// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperTool.h"
#include "MinesweeperToolStyle.h"
#include "MinesweeperToolCommands.h"
#include "LevelEditor.h"
#include "SMinesweeperWindow.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MinesweeperTabName("MinesweeperTool");

#define LOCTEXT_NAMESPACE "FMinesweeperToolModule"

void FMinesweeperToolModule::StartupModule()
{
    // Register the styles and commands for our plugin
    FMinesweeperToolStyle::Initialize();
    FMinesweeperToolStyle::ReloadTextures();

    FMinesweeperToolCommands::Register();
    
    // Register plugin callbacks
    PluginCommands = MakeShareable(new FUICommandList);
    
    // Map the plugin button to our callback
    PluginCommands->MapAction(
        FMinesweeperToolCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateRaw(this, &FMinesweeperToolModule::PluginButtonClicked),
        FCanExecuteAction());
    
    // Register menus
    RegisterMenus();
    
    // Register a nomad tab spawner for our plugin window
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName, 
        FOnSpawnTab::CreateRaw(this, &FMinesweeperToolModule::OnSpawnPluginTab))
        .SetDisplayName(LOCTEXT("MinesweeperTabTitle", "Minesweeper"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperToolModule::ShutdownModule()
{
    // Unregister all the resources we've registered
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
    
    FMinesweeperToolStyle::Shutdown();
    FMinesweeperToolCommands::Unregister();
    
    // Unregister tab spawner
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

void FMinesweeperToolModule::PluginButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperToolModule::RegisterMenus()
{
    // Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
    FToolMenuOwnerScoped OwnerScoped(this);
    
    // Register the "Minesweeper" button in the toolbar
    {
        UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
        FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Minesweeper");
        {
            FToolMenuEntry& Entry = Section.AddEntry(
                FToolMenuEntry::InitToolBarButton(
                    FMinesweeperToolCommands::Get().OpenPluginWindow,
                    FText::GetEmpty(),
                    LOCTEXT("MinesweeperToolbarTooltip", "Open the Minesweeper game"),
                    FSlateIcon(FMinesweeperToolStyle::GetStyleSetName(), "MinesweeperTool.PluginAction")
                )
            );
            Entry.SetCommandList(PluginCommands);
        }
    }
    
    // Register the "Minesweeper" menu entry
    {
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
        FToolMenuSection& Section = Menu->FindOrAddSection("MinesweeperTools");
        Section.AddMenuEntryWithCommandList(
            FMinesweeperToolCommands::Get().OpenPluginWindow,
            PluginCommands,
            LOCTEXT("MinesweeperMenuLabel", "Minesweeper"),
            LOCTEXT("MinesweeperMenuTooltip", "Open Minesweeper game"),
            FSlateIcon(FMinesweeperToolStyle::GetStyleSetName(), "MinesweeperTool.PluginAction")
        );
    }
}

TSharedRef<SDockTab> FMinesweeperToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    // Create the Minesweeper window
    TSharedRef<SDockTab> DockTab = SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SBox)
            .MinDesiredWidth(400)
            .MinDesiredHeight(500)
            [
                SNew(SMinesweeperWindow)
            ]
        ];
    
    // Store the tab so we can refer to it later if needed
    MinesweeperTab = DockTab;
    
    return DockTab;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FMinesweeperToolModule, MinesweeperTool)