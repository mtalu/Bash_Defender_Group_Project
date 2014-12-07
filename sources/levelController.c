/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers -----------*/

#include "../includes/debug.h"
#include "../includes/sput.h"
#include "../includes/levelController.h"

/*---------- Functions ----------*/
int SCREEN_WIDTH_GLOBAL;
int SCREEN_HEIGHT_GLOBAL;

/*
 *Initializes all data structures required for level
 */
void initLevel()    {

    createLevelPaths();
    createTowerGroup();
    createActionQueue();
    createGame();
    createEnemyGroup();
	createTowerPos();
}

void levelOneTowerPostions()	{

	addTowerPosNode(100,200);	
	addTowerPosNode(300,600);
	addTowerPosNode(500,300);
}

/*
 *Frees all Data structures
 */
void endLevel() {

    freeAllTowers();
    free(getTowerGrp(NULL));
    free(getGame(NULL));
    free(getQueue(NULL));
	freeAllTowerPositions();
	freeEnemyGroup();
	freeLevelPaths();
}

void initEnemy()  {
    //! function should be based on level and wave number as to how many enemies are created
    //
}
