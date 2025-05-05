// MinesweeperToolStyle.h
#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * Defines the visual style for the Minesweeper Tool plugin
 */
class FMinesweeperToolStyle
{
public:
	static void Initialize();
	static void Shutdown();
    
	/** @return The Slate style set for the plugin */
	static TSharedRef<FSlateStyleSet> Create();
    
	/** @return The name of the style set */
	static FName GetStyleSetName();
    
	/** Reload textures used by the style set */
	static void ReloadTextures();

private:
	/** Singleton instance of the style */
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};