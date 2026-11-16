//---------------------------------------------------------
// file:	screen.c
// author:  Claire Kuan Wai Cheong
// email:   c.cheong@digipen.edu
//
// brief:   Initialising screens (Giving integer value for page
//			calling)
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "screen.h"

// Initialize the current screen to the main menu
int currentScreen = SCREEN_MAIN_MENU;
int gameScreen = SCREEN_GAME;
int winScreen = SCREEN_WIN;
int loseScreen = SCREEN_LOSE;
int introductionScreen = SCREEN_INTRODUCTION;
int winScreenLevel = SCREEN_WIN_LEVEL;