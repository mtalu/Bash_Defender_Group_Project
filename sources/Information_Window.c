//
//  Information_Window.c
//  Group_Project
//
//  Functions relating to tower monitor and stats monitor
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "../includes/Information_Window.h"
#include "../includes/tower.h"
#include "../includes/gameProperties.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT_STRING 200


void towerMonitor(unsigned int targetTower, char *optionalOutputString) {
    int time = SDL_GetTicks();
    static int lastTower = 0, timeOfCall = 0, optionalStringSet = 0;
    char *outputString;
    static char *optionalString = NULL;

    if(targetTower && !optionalOutputString) {
        outputString = getTowerString(targetTower);
        timeOfCall = SDL_GetTicks();
        lastTower = targetTower;
        optionalStringSet = 0;
    }
    else if(optionalOutputString) {
        outputString = optionalString = optionalOutputString;
        timeOfCall = SDL_GetTicks();
        optionalStringSet = 1;
    }
    else {
        outputString = getDefaultTowerString();
    }
    
    if(timeOfCall != 0 && time - timeOfCall < 10000) {
        if(optionalStringSet) {
            outputString = optionalString;
        }
        else {
            outputString = getTowerString(lastTower);
        }
    }
    
    if(time - timeOfCall > 10000) {
        optionalStringSet = 0;
    }
    
        updateTowerMonitor(outputString);
}

char *getDefaultTowerString() {
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    
    return outputString;
}

char *getTowerString(unsigned int targetTower) {
    
    int range, damage, speed, AOEpower, AOErange;
    getStats(&range, &damage, &speed, &AOEpower, &AOErange, targetTower);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "TOWER %d\n\nRange: %d\nDamage: %d\nAOE Speed: %d\nAOE Power: %d\nAOE Range: %d", targetTower, range, damage, speed, AOEpower, AOErange);
    
    return outputString;
}

void statsMonitor() {
    
    GameProperties properties = getGame(NULL);
    
    int gold = getGold(properties);
    int waveNumber = getWave(properties);
    int health = getHealth(properties);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "\n               STATS MONITOR\n\nGold: %d\nWave Number: %d\nHealth: %d", gold, waveNumber, health);
    
    updateStatsMonitor(outputString);
}

void manUpgrade()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nupgrade\n\n type ""upgrade"" followed by a stat\n ( p, r, s, AOEp, AOEr)\n ) followed by a target tower\neg t1, t2, t3...\nExamples:\nupgrade r t2\nupgrade p t3");
}

void manCat()
{
    
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: cat \n type ""cat"" followed by a target eg t1, t2, t3... to display the stats of that target\n");
}
void manMan()
{
    towerMonitor(-1, "GENERAL COMMANDS MANUAL: man \n type ""man"" followed by a command eg upgrade or cat to view the manual entry for that command\n");
}


