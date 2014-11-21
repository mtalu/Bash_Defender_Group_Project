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
       //Create output string with tower information and send to graphics module
    struct tower towerToCat = getStats(towerIDtoCat);
    
    char outputString[200];
    
    char range[10], damage[10], speed[10], AOEdamage[10], AOErange[10];
    
    sprintf(outputString, "  TOWER %d\n\nRange: ", 1);
    sprintf(range, "%d", towerToCat.range);
    sprintf(damage, "%d", towerToCat.damage);
    sprintf(speed, "%d", towerToCat.speed);
    sprintf(AOEdamage, "%d", towerToCat.AOEpower);
    sprintf(AOErange, "%d", towerToCat.AOErange);

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
