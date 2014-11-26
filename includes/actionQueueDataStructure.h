#ifndef actionQueueDataStructure_h
#define actionQueueDataStructure_h

#include "./../includes/debug.h"
#include "./../includes/gameProperties.h"

/*----------Enumerated Types-----------*/
enum commandType
{
    commandError=-1,
    upgrade=0,
    execute=1,
    set=2,
    man=3,
    cat=4,
    mktwr=5
};
typedef enum commandType commandType;

enum upgradeStat
{
    statError=-1,
    power=0,
    range=1,
    speed=2,
    AOErange=3,
    AOEpower=4
};
typedef enum upgradeStat upgradeStat;

/*----------Symbolic Constants-----------*/

/*----------TypeDefs-----------*/

typedef struct queueNode *QueueNode;
typedef struct actionQueueStructure *ActionQueueStructure;

/*----------Function Prototypes-----------*/
int checkTime(GameProperties Game);
int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed);
ActionQueueStructure createActionQueue();
int createNode(ActionQueueStructure queue);
int pushToQueue(ActionQueueStructure queue, commandType command, upgradeStat option, int target);
int popFromQueue(ActionQueueStructure queue, commandType *cmd, upgradeStat *stat, int *target, GameProperties Game, int needed);
int checkGold(int needed, GameProperties Game);
commandType getFirstCommand(ActionQueueStructure queue);
commandType getLastCommand(ActionQueueStructure queue);
upgradeStat getFirstOption(ActionQueueStructure queue);
upgradeStat getLastOption(ActionQueueStructure queue);
ActionQueueStructure getQueue(ActionQueueStructure queue);
int getLastTarget();
int getFirstTarget();

void testcheckGold();
void testingActionQueue();
void testPopFromQueue();
void testPushToQueue();

#endif
