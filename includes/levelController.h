#ifndef actionQueueDataStructure_h

/*---------- Custom Headers -----------*/
#include "../includes/tower.h"
#include "../includes/actionQueueDataStructure.h"

#define ENDOFSTRING '\0'
#define READCHAR	'/'
typedef enum levelCommand { makeTowerP, totalWaves, wave, delay } levelCommand;
typedef enum property {x, y, total, waveID, enemyType, dTime } property;

typedef struct keyword *Keyword;
typedef struct lCmdProp *LCmdProp;
typedef struct keywordQueue *KeywordQueue;
typedef struct parseLineArray *ParseLineArray;

/*---------- Functions ----------*/
void createKeywordQueue();
KeywordQueue getKWQueue(KeywordQueue kwQueue);
void initLevel();
void endLevel();
void levelSettings();
void addKeyWordToken(char *token);
void readLevelSettingsFile(char *file);
int checkProperty(char *token);
int checkForSemiColon(char *token);
void addKWtoQueue(Keyword nwKW);
char* expandCBuffer(char *toExpand, int currSize);
char* getToken(char *line);
int validateLine(char *Line, int nWords);
void addProperty(property p);
void addValue(char *token);
void createLevel();
void printQueue();
#endif
