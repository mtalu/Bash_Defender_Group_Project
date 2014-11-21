#ifndef TEST_Header_h
#define TEST_Header_h

#include "../includes/enemy.h"
#include "../includes/gameProperties.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/Display.h"

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
};

typedef struct tower *tower;
typedef struct towerGroup *TowerGroup;

tower createTower();
int upgradeDmg(int target);
void createTowerGroup();
TowerGroup getTowerGrp();
tower getTowerID(int target);
tower getTowerPointer(tower updatedT);
void populateTower(tower newTow, int id);
int getRange();
int getTowerX();
int getTowerY();
int isFiring();
int firingX();
int firingY();
void freeTower(tower t);
void fire();
void printTower(tower t);
commandType checkActQueue();
upgradeStat upgradeTowerStat(upgradeStat stat,int target);
unsigned int getNumberOfTowers();
void freeAllTowers();
void present_tower(Display d);
struct tower getStats(unsigned int towerID);

void testingTowerModule();
void testGetTower();
void testUpgradeTowerStat();
#endif

