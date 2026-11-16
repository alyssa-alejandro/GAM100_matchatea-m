//---------------------------------------------------------
// file:	main.c
// author:	Claire Kuan Wai Cheong
// email:	c.cheong@digipen.edu
//
// brief:	Entry point for game
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "mainmenu.h"
#include "game.h"
#include "credits.h"

int main(void)
{
	CP_Engine_SetNextGameState(Intro_Credits_Init, Intro_Credits_Update, Intro_Credits_Exit);
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_Run(0);
	return 0;
}
