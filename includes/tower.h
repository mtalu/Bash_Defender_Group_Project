#ifndef TEST_Header_h
#define TEST_Header_h

#include "../includes/enemy.h"
#include "../includes/gameProperties.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/Display.h"

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

void testingTowerModule();
void testGetTower();
void testUpgradeTowerStat();
#endif

