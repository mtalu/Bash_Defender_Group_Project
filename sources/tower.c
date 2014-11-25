#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 300

#include "../includes/tower.h"
#include "../includes/sput.h"
#include "../includes/debug.h"

struct tower {
	int towerID;
    int x, y;
    int damage;
    int range;
	int speed;
    int AOErange; //! not yet implemented
	int AOEpower; //!not yet implemented
	int firing;
    int targetPosition[2];
	int level;
};

void testingTowerModule()	{
	sput_start_testing();
	sput_set_output_stream(NULL);	

	sput_enter_suite("testGetTower(): Tower creation and being placed in tower array");
	sput_run_test(testGetTower);
	sput_leave_suite();
	
	sput_enter_suite("testUpgradeTowerStat(): Upgrading tower stats");
	sput_run_test(testUpgradeTowerStat);
	sput_leave_suite();

	sput_finish_testing();

}


struct towerGroup	{

	tower *listOfTowers;
	unsigned int numOfTowers;
	
};

int getTowerDamage(int towerID)	{
	return getTowerGrp(NULL)->listOfTowers[towerID-1]->damage;
}

void createTowerGroup()	{

	TowerGroup Group = (TowerGroup) malloc(sizeof(*Group));
	getTowerGrp(Group);
	Group->listOfTowers=malloc(sizeof(tower));
	Group->numOfTowers = 0;
}
/* original create tower may be decomissioned in future
    returns the number of towers if succesful 
    returns 0 if something goes wrong
 */
tower createTower() {
    getTowerGrp(NULL)->numOfTowers++; //!increased number of towers when one is created
    getTowerGrp(NULL)->listOfTowers = realloc(getTowerGrp(NULL)->listOfTowers, (getTowerGrp(NULL)->numOfTowers+1)*sizeof(tower));
    if(getTowerGrp(NULL)->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    getTowerGrp(NULL)->listOfTowers[getTowerGrp(NULL)->numOfTowers] = t;
    
    getTowerPointer(t);  //! Should no longer be used.  Functions should ID tower that they wish to target.
    
    populateTower(t,getTowerGrp(NULL)->numOfTowers); //! populating tower stats
	

    return t;
	//return getTowerGrp(NULL)->numOfTowers;
    
}
/* called when create tower command input by player. Places a tower at the specified x y.
    returns total number of towers if succesful
    returns 0 if failled
 */
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY)
{
    TowerGroup TG = getTowerGrp(NULL);

    TG->numOfTowers++; //!increased number of towers when one is created
    TG->listOfTowers = realloc(TG->listOfTowers, (TG->numOfTowers+1)*sizeof(tower));
    if(TG->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    TG->listOfTowers[TG->numOfTowers] = t;
    initialiseNewTower(t, inputTowerPositionX, inputTowerPositionY);
    
    
    return TG->numOfTowers;
    
}
void initialiseNewTower(tower newTow, int TowerPositionX, int TowerPositionY )
{
    TowerGroup TG = getTowerGrp(NULL);
    
    newTow->towerID = TG->numOfTowers;//new tower ID is the same as the number of towers in the group
    newTow->x = TowerPositionX;
    newTow->y = TowerPositionY;
    newTow->damage = 10;
    newTow->range = 10;
    newTow->firing = 0;
	newTow->level = 1;
    newTow->speed = 10;
    newTow->AOEpower = 10;
    newTow->AOErange = 10;
    
}


/*
 * Must be called and created before towers are created.
 */
TowerGroup getTowerGrp(TowerGroup Group)	{

	static TowerGroup newGroup;

	if(Group != NULL)	{
		newGroup = Group;
	}

	return newGroup;
}

commandType checkActQueue()
{
	commandType cmd;
	upgradeStat stat;
	int target;
	if (popFromQueue(getQueue(NULL),&cmd,&stat,&target,getGame(NULL),10))
    {
		switch (cmd)
        {
			case upgrade:
            {
					if( upgradeTowerStat(stat,target)!=-1 ) 	{
						return upgrade;
					}
                    else {
						return 0;
					}
            }
			case execute:
					return execute;
					break;
			default:	
					fprintf(stderr,"checkActQueue tower.c: unrecognised command\n");
					break;
		}
	}
	return 0;
}

int upgradeDmg(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->damage++;
	}
	return 0;
}
int upgradeRange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->range++;
	}
	return 0;
}
int upgradeSpeed(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->speed++;
	}
	return 0;
}
int upgradeAOEpower(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->AOEpower++;
	}
	return 0;
}
int upgradeAOErange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		return upgradeT->AOErange++;
	}
	return 0;
}

upgradeStat upgradeTowerStat(upgradeStat stat, int target)	{

	switch(stat)	{
		case power:
        {
			if(upgradeDmg(target))	{
				return power;
			}
        }
		case range:
        {
			if(upgradeRange(target))	{
				return range;
			}
        }
		case speed:
        {
			if(upgradeSpeed(target))	{
				return speed;
			}
        }
		case AOErange:
        {
			if(upgradeAOErange(target))	{
				return AOErange;
			}
        }
		case AOEpower:
        {
			if(upgradeAOEpower(target))	{
				return AOEpower;
			}
        }
		default:
			fprintf(stderr,"upgradeTowerStat tower.c: unrecognised stat\n");
            return statError;

	}
}


void testUpgradeTowerStat()	{

	createLevelPaths();
	createTowerGroup();
	tower t1 = createTower();
	tower t2 = createTower();
	sput_fail_unless(upgradeTowerStat(power,1) == power,"Valid Power Upgrade: tower one for upgrade");
	sput_fail_unless(upgradeTowerStat(power,1) == power,"Valid Power Upgrade: Tower two for upgrade");
	sput_fail_unless(upgradeTowerStat(0,1) == 0, " Invalid: Passing invalid upgrade");
	sput_fail_unless(upgradeTowerStat(power,3) == 0, " Invalid: Passing invalid target");
	free(t1);
	free(t2);
}

unsigned int getNumberOfTowers()	{
	return ((getTowerGrp(NULL))->numOfTowers);
}


void freeAllTowers()	{

	int i = 0;
	while(i < getTowerGrp(NULL)->numOfTowers)	{
		free(getTowerGrp(NULL)->listOfTowers[i]);
		i++;
	}
}

void testGetTower()	{

	createLevelPaths();
	createTowerGroup();
	createTower();
	sput_fail_unless(getNumberOfTowers() == 1, "Valid: Number of towers held in group is one.");
	sput_fail_unless(getTowerID(1) != NULL,"Valid: Tower with ID 1 exists.");
	createTower();
	sput_fail_unless(getNumberOfTowers() == 2, "Valid: Number of towers held in group is two");
	sput_fail_unless(getTowerID(2) != NULL,"Valid: Tower with ID 2 exists.");
	freeAllTowers();
	free(getTowerGrp(NULL));
}

tower getTowerPointer(tower updatedT) {
    
    static tower t;
    
    if(updatedT != NULL) {
        t = updatedT;
    }
    
    return t;
}

/*
 *Returns tower based on ID
 */
tower getTowerID(int target)	{
	int i;
	for( i = 0; i < (getTowerGrp(NULL))->numOfTowers; i++)	{
		if((getTowerGrp(NULL))->listOfTowers[i]->towerID == target)	{
				return getTowerGrp(NULL)->listOfTowers[i];
		}
	}

	return NULL;
}



void populateTower(tower newTow, int id) {
   	
    newTow->towerID = id;
    newTow->x = SCREEN_WIDTH/2;
    newTow->y = (SCREEN_HEIGHT/2)+10;
    newTow->damage = 10;
    newTow->range = 10;
    newTow->firing = 0;
	newTow->level = 1;
    newTow->speed = 50;


}

void getStats(int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID)
{
    TowerGroup towers = getTowerGrp(NULL);
     *range = towers->listOfTowers[towerID-1]->range;
     *damage = towers->listOfTowers[towerID-1]->damage;
     *speed = towers->listOfTowers[towerID-1]->speed;
     *AOEpower = towers->listOfTowers[towerID-1]->AOEpower;
     *AOErange = towers->listOfTowers[towerID-1]->AOErange;
}

int getTowerX(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->x;
}

int getTowerY(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->y;
}

int setTowerY(int towerID, int newY)	{

	getTowerGrp(NULL)->listOfTowers[towerID-1]->y = newY;
	return newY;
	
}

int setTowerRange(int towerID, int newRange)	{

	getTowerGrp(NULL)->listOfTowers[towerID-1]->range = newRange;
	return newRange;

}

int setTowerX(int towerID,int newX)	{

	getTowerGrp(NULL)->listOfTowers[towerID-1]->x = newX;
	return newX;

}

int isFiring() {
    
    tower t = getTowerPointer(NULL);
    
    return t->firing;
}

int firingX() {
    tower t = getTowerPointer(NULL);

    return t->targetPosition[0];

}

int firingY() {
    tower t = getTowerPointer(NULL);
    
    return t->targetPosition[1];
}


void freeTower(tower t) {
  free(t);
}

void fire() {
  
	int enemyID, towerID;
	for(towerID = 1; towerID <= getNumberOfTowers(); towerID++)	{ 
		for(enemyID = 1; enemyID <= getNumberOfEnemies(); enemyID++)	{
			if(inRange(getTowerID(towerID)->x, getTowerID(towerID)->y, getTowerID(towerID)->range, enemyID) == 1) {
				(getTowerID(towerID))->firing = 1;
				towerGetTargetPos(getTowerID(towerID)->targetPosition,enemyID);	
				damageEnemy (getTowerID(towerID)->damage,enemyID);
			} else {
				(getTowerID(towerID))->firing = 0;
			}
		}
	}
//  if(inRange(t->x, t->y, t->range) == 1) {
//      t->firing = 1;
//      getTarget(&t->target[0]);
//      damageEnemy (t->damage);
//  } else {
//      t->firing = 0;
//  }
}


void printTower(tower t) {

  printf("tower x = %d, tower y = %d, tower firing = %d", t->x, t->y, t->firing);
  if(t->firing) {
    printf(" Tower firing coords x = %d, tower firing coords y = %d ", t->targetPosition[0], t->targetPosition[1]);
  } else {
    printf(" ");
  }
}

void present_tower(Display d)
{
    TowerGroup TG = getTowerGrp(NULL);
    if(TG->numOfTowers>0)
    {
        for(int towerID=1; towerID<=TG->numOfTowers; ++towerID)
        {
            drawTower(d, getTowerX(towerID), getTowerY(towerID), 80, 80);
            // 80s for tow width and height - these are constant for now.
        }

    }
    
}

