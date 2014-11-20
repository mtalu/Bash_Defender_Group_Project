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
void displayTowerInformation(Display d) {
    
    //Create output string with tower information and send to graphics module
    
    char outputString[200];
    
    char range[10], damage[10], firing[10];
    
    sprintf(outputString, "  TOWER %d\n\nRange: ", 1);
    sprintf(range, "%d", getRange());
    sprintf(damage, "%d", getRange());
    sprintf(firing, "%d", getRange());
    
    strcat(outputString, range);
    strcat(outputString, "\nDamage : ");
    strcat(outputString, damage);
    strcat(outputString, "\nFiring: ");
    strcat(outputString, firing);
    
    updateInfoWindow(d, outputString);
}
