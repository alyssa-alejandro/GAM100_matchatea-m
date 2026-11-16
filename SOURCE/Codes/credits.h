//---------------------------------------------------------
// file:	credits.h
// author:  Low Jia En
// email:   jiaen.low@digipen.edu
//
// brief:   Contains declaration for opening and closing credits
//			functions defined in credits.c.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "cprocessing.h"
#include <stdlib.h>  

void Intro_Credits_Init(void);
void Intro_Credits_Update(void);
void Intro_Credits_Exit(void);

void End_Credits_Init(void);
void End_Credits_Update(void);
void End_Credits_Exit(void);