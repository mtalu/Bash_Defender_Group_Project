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

void catTower(unsigned int towerIDtoCat)
{
    printf("tow id %d",towerIDtoCat);
    int iRange, iDamage, iSpeed, iAOEpower, iAOErange;
       //Create output string with tower information and send to graphics module
    getStats(&iRange,&iDamage,&iSpeed,&iAOEpower,&iAOErange, towerIDtoCat);
    
    char outputString[200];
    
    char range[10], damage[10], speed[10], AOEdamage[10], AOErange[10];
    
    sprintf(outputString, "  TOWER %d\n\nRange: ", 1);
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

void manUpgrade()
{
    updateInfoWindow("GENERAL COMMANDS MANUAL: upgrade\n type ""upgrade"" followed by a stat\n ( p, r, s, AOEp, AOEr)\n ) followed by a target tower\neg t1, t2, t3...\nExamples:\nupgrade r t2\nupgrade p t3");
}

void manCat()
{
    updateInfoWindow("GENERAL COMMANDS MANUAL: cat \n type ""cat"" followed by a target eg t1, t2, t3... to display the stats of that target\n");
}
void manMan()
{
    updateInfoWindow("GENERAL COMMANDS MANUAL: man \n type ""man"" followed by a command eg upgrade or cat to view the manual entry for that command\n");
}

