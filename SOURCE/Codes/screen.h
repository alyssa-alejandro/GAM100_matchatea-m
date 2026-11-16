//---------------------------------------------------------
// file:	screen.h
// author:  Claire Kuan Wai Cheong
// email:   c.cheong@digipen.edu
//
// brief:   Contains variable declaration defined in screen.c
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#ifndef SCREENS_H
#define SCREENS_H

// Screen state constants
#define SCREEN_MAIN_MENU 0
#define SCREEN_GAME 1
#define SCREEN_WIN 2
#define SCREEN_LOSE 3
#define SCREEN_INTRODUCTION 4
#define SCREEN_WIN_LEVEL 5

// Track the current screen state
extern int currentScreen;
extern int gameScreen;
extern int winScreen;
extern int loseScreen;
extern int introductionScreen;
extern int winLevelScreen;

#endif
