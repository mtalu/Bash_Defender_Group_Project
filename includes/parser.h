//
//  parser.h
//  
//
//  Created by ben on 07/11/2014.
//
//

#ifndef _parser_h
#define _parser_h

#include "../includes/actionQueueDataStructure.h"

#define ENABLE_TESTING 0

int getAction( const char * inputAction );
char * strdup(const char * s);
int parse(char *inputString);
void freeCommandArray(char **commandArray,int numberOfChunks);
char ** breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter);
void testCommandArray(char ** commandArray, int numberOfChunks);
void testGetAction(enum commandType action);
void actionUsageError( commandType action, const char ** validActions, int numberOfActions);
upgradeStat getUpgradeStats(const char * inputStringUpgradeStats);
void parseUpgrade(char ** commandArray, int numberOfChunks);
void testGetUpgradeStat(upgradeStat statToUpgrade);
void upgrageStatUsageError( upgradeStat statToUpgrade, const char ** validUpgradeStats, int numberOfStats);
unsigned int getTargetTower(const char * inputStringTargeting);
void parseCat(char * inputStringTargeting);
void parseMan(char * inputStringCommandMan);
void targetTowerError(unsigned int numberOfTowers, unsigned int targetTower);



#endif


