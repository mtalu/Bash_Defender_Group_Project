//
//  Information_Window.c
//  Group_Project
//
//  Library with all functions to create and update information window.
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "../includes/Information_Window.h"
#include "../includes/tower.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void catTower(unsigned int towerIDtoCat) {
    printf("tow id %d",towerIDtoCat);
    int iRange, iDamage, iSpeed, iAOEpower, iAOErange;
       //Create output string with tower information and send to graphics module
    getStats(&iRange,&iDamage,&iSpeed,&iAOEpower,&iAOErange, towerIDtoCat);
    
    char outputString[200];
    
    char range[10], damage[10], speed[10], AOEdamage[10], AOErange[10];
    
    sprintf(outputString, "TOWER %d\n\nRange: ", 1);
    sprintf(range, "%d", iRange);
    sprintf(damage, "%d",iDamage);
    sprintf(speed, "%d", iSpeed);
    sprintf(AOEdamage, "%d",iAOEpower);
    sprintf(AOErange, "%d", iAOErange);

    strcat(outputString, range);
    strcat(outputString, "\nDamage : ");
    strcat(outputString, damage);
    strcat(outputString, "\nSpeed: ");
    strcat(outputString, speed);
    strcat(outputString, "\nAOE damage: ");
    strcat(outputString, AOEdamage);
    strcat(outputString, "\nAOE range: ");
    strcat(outputString, AOErange);
    
    updateInfoWindow(outputString);
}

void statsMonitor() {
    int iGold = 10;
    
    char outputString[200];
    
    char gold[10];
    
    sprintf(outputString, "        STATS MONITOR\n\nGold: ");
    sprintf(gold, "%d", iGold);
    
    strcat(outputString, gold);
    
    updateStatsMonitor(outputString);
}


