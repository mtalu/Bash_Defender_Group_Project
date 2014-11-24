//
//  Display.h
//  TEST
//
//  Created by Vlad Vyshnevskyy on 19/11/2014.
//  Copyright (c) 2014 VV-SD. All rights reserved.
//

#ifndef TEST_Display_h
#define TEST_Display_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>


#include <stdlib.h>
#include <string.h>

typedef struct display *Display;
int processEvents(Display d);


Display init_SDL();

Display getDisplayPointer(Display d);


void startFrame(Display d);
void endFrame(Display d);
void shutSDL(Display d);



//tower
void init_tower(Display d, char *pic_name);
void drawTower(Display d, int x, int y, int w, int h);

//enemy
void init_enemy(Display d, char *pic_name);
void drawEnemy(Display d, int x, int y, int w, int h);


//check
void check_load_images(SDL_Surface *surface, char *pic_name);

TTF_Font *getInfoWindowFont(TTF_Font *font);


#include <stdio.h>
#include "Information_Window.h"

void displayInfoWindow(Display d);
void updateInfoWindow( char *outputString);
void sendTextToInfoWindow(Display d, char *string);
char *strdup2(char * s);


int terminal_window(Display d, char *pass, char *clear, char* inputCommand);
void display_text(Display d, char *pass);

#endif

// for my purpose...
//void present_tower(Display d){
//    static int done = 0;
//    if (!done) {
//        init_tower(d, "tower.png");
//        done = 1;
//    }
//    drawTower(d, 80, 100);
//}
