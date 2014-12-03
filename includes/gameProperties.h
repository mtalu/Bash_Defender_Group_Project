#ifndef gameProperties_h
#define gameProperties_h
#include <time.h>

#include "./../includes/debug.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define ACTIONCOOLDOWN	1	//! minimum time between actions being removed from queue

/*----------TypeDefs----------*/

typedef struct gameProperties *GameProperties;
typedef struct gameClock *GameClock;
/*----------Function Prototypes-----------*/
GameProperties createGame();
int getGold(GameProperties game);
int takeGold(GameProperties game,int gold);
int getWave(GameProperties game);
int getHealth(GameProperties game);
clock_t delayGame(int delayN);
int lastAction(GameProperties Game);
int setlastAction(GameProperties Game);
GameProperties getGame(GameProperties createdGame);
void damageHealth(int damage);
int addGold(int gold);

void testingGameStructure();
void CreateGameTest();
void TestGetGold();
void TestAddGold();
void TestTakeGold();

#endif
