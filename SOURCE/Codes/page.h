//---------------------------------------------------------
// file:	page.h	
// author:  Claire Kuan Wai Cheong
// email:   c.cheong@digipen.edu
// 
// brief:	Contains function declaration for win and losing a 
//			level defined in page.c.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

void Win(game_score* score);
void Win_Level(game_score *score);
void Lose(game_score *score);
