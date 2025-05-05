# Minesweeper Game - Unreal Engine Editor Tool

This repository contains a Minesweeper game implemented as an Unreal Engine Editor tool using C++ and Slate UI framework.

## Overview

This plugin adds a Minesweeper game that can be played directly within the Unreal Editor. It's implemented as an editor utility plugin using C++ and Slate (no UMG or Editor Utility Widgets).

## Features

- Minesweeper game accessible via toolbar button
- Configurable game settings:
  - Grid width
  - Grid height
  - Number of bombs
- Classic Minesweeper gameplay:
  - Left-click to reveal tiles
  - Numbers showing adjacent bombs
  - Auto-reveal of empty regions
  - Game over detection
- New game functionality

## Requirements

- Unreal Engine 5.3
- Visual Studio 2022 (or compatible C++ compiler)

## Installation Instructions

1. Clone this repository into your project's `Plugins` folder:
2. git clone https://github.com/Yagyam1234/MineSweeperGame.git Plugins/MinesweeperTool
2. Regenerate your project files (right-click on .uproject file and select "Generate Visual Studio project files")
3. Open your project in Unreal Engine 5.3
4. Enable the plugin in Edit > Plugins > Editor Utilities > Minesweeper Tool

## Usage

1. Click the Minesweeper icon in the editor toolbar (or go to Tools > Minesweeper)
2. Configure your desired grid size and bomb count
3. Click "New Game" to start
4. Left-click tiles to reveal them
5. Try to reveal all non-bomb tiles to win!

## Implementation Details

The plugin is structured as follows:
- `MinesweeperGame` - Core game logic implementation
- `SMinesweeperWindow` - Main game window UI
- `SMinesweeperTile` - Individual tile UI component
- `MinesweeperToolModule` - Plugin registration and integration

## Acknowledgments

This project was developed as a demonstration of C++ and Slate UI programming in Unreal Engine.
