#include <stdio.h>
#include <stdlib.h>
#include "../includes/sput.h"
#include <math.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 300

#include "../includes/enemy.h"

struct path {
  int pathLength;
  int **pathCoords;
} ;


struct levelPaths {
  int numberOfPaths;
  Path *paths;
} ;

struct enemy {
    int x, y;
    Path enemyPath;
    int pathProgress;
    int maxHealth;
    int health;
    int armour;
    int speed;
    int damage;
    int enemyID;
	  BOOL firedUpon;
    int dead;
};

struct enemyGroup {
    int numberOfEnemies;
    Enemy * enemyArray;
};


void createLevelPaths()
{
  LevelPaths pathList = (LevelPaths) malloc(sizeof(*pathList));
  getLevelPaths(pathList);
  int numberOfPaths = 2;
  
  
  assignMemoryForPaths(numberOfPaths);
  layPaths(numberOfPaths);
  
}

void layPaths(int numberOfPaths)
{
  
  LevelPaths lP = getLevelPaths(NULL);
  switch(numberOfPaths) {
    case 1 :
      createHorizontalPath(lP->paths[0]);
      break;
    case 2 :
      createHorizontalPath(lP->paths[0]);
      createDogLegPath(lP->paths[1]);
      break;
      
    default :
      fprintf(stderr,"****ERROR failed to read the number of required paths****\n");
      exit(2);
  }
}

void createHorizontalPath(Path P)
{
  //create one path all the way across the middle of the screen
  P->pathLength = 0;
  P->pathCoords = (int **) malloc(sizeof(int *) * SCREEN_WIDTH);
  for(int i = 0; i < SCREEN_WIDTH; i++) {
    P->pathCoords[i] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[i][0] = i;
    P->pathCoords[i][1] = SCREEN_HEIGHT/2;
    P->pathLength++;
  }
}

void createDogLegPath(Path P)
{
  //create path that goes up from the centre and carries along a route above the middle of the screen
  P->pathLength = 0;
  P->pathCoords = (int **) malloc(sizeof(int *) * (SCREEN_WIDTH + (SCREEN_HEIGHT/4))); //length of path should be width and a quarter of height
    // go right for a quarter of the screen width
  for (int i = 0; i < SCREEN_WIDTH/4; i++) {
    P->pathCoords[P->pathLength] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[P->pathLength][0] = i;
    P->pathCoords[P->pathLength][1] = SCREEN_HEIGHT/2;
    P->pathLength++;
  }
    // go up for a quarter of the screen height
  for(int i = SCREEN_HEIGHT/2; i < 3*SCREEN_HEIGHT/4; i++) {
    P->pathCoords[P->pathLength] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[P->pathLength][0] = P->pathCoords[P->pathLength-1][0];
    P->pathCoords[P->pathLength][1] = i;
    P->pathLength++;
  }
  
    // carry on right for the rest of the screen width
  for(int i = SCREEN_WIDTH/4; i < SCREEN_WIDTH; i++) {
    P->pathCoords[P->pathLength] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[P->pathLength][0] = i;
    P->pathCoords[P->pathLength][1] = P->pathCoords[P->pathLength-1][1];
    P->pathLength++;
  }
}
    


void assignMemoryForPaths(int numberOfPaths)
{

  LevelPaths lP = getLevelPaths(NULL);
  lP->numberOfPaths = numberOfPaths;
  lP->paths = (Path *)malloc(sizeof(Path) * numberOfPaths);
  if(lP->paths == NULL) {
    fprintf(stderr,"****ERROR malloc in create path pointers failed****\n");
    exit(1);
  }
  for(int i = 0; i < numberOfPaths; i++) {
    lP->paths[i] = (Path)malloc(sizeof(struct path));
  }
  
}

LevelPaths getLevelPaths(LevelPaths pathList)
{
    
    static LevelPaths lP;
    
    if(pathList != NULL) {
        lP = pathList;
    }
    
    return lP;
}

void createEnemyGroup()
{
    EnemyGroup enemyList = (EnemyGroup) malloc(sizeof(*enemyList));
    getEnemyGroup(enemyList);
    enemyList->enemyArray=malloc(sizeof(Enemy));
    enemyList->numberOfEnemies = 0;
}

void createEnemy()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    ++(enemyList->numberOfEnemies);
    enemyList->enemyArray = (Enemy*)realloc(enemyList->enemyArray, (enemyList->numberOfEnemies+1)*(sizeof(Enemy)));

    if(enemyList->enemyArray==NULL)
    {
	printf("****ERROR realloc in createEnemy failed****\n");
	exit(1);
    }
    enemyList->enemyArray[enemyList->numberOfEnemies]=(Enemy)malloc(sizeof(struct enemy));
		        
    if( enemyList->enemyArray[enemyList->numberOfEnemies]==NULL)
    {
	printf("****ERROR malloc in createEnemy failed****\n");
        exit(1);
    }
   initialiseEnemy( enemyList->enemyArray[enemyList->numberOfEnemies]);
}
int getNumberOfEnemies()
{
    return getEnemyGroup(NULL)->numberOfEnemies;
    
}

void Test_createEnemy()
{
    
    createLevelPaths();
    createEnemyGroup();
    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 1, "Valid: Number of enemies held in group is one.");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy healt is default." );

    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 2, "Valid: Number of enemies held in group is two.");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy 2  health is default." );


}

                                            
void initialiseEnemy(Enemy newEnemy)
{
    
    LevelPaths lP = getLevelPaths(NULL);
    newEnemy->enemyPath = lP->paths[rand()%lP->numberOfPaths];
    newEnemy->pathProgress = 0;
    newEnemy->x = newEnemy->enemyPath->pathCoords[0][0];
    newEnemy->y = newEnemy->enemyPath->pathCoords[0][1];
    newEnemy->maxHealth = 100;
    newEnemy->health = newEnemy->maxHealth;
    newEnemy->armour = 1;
    newEnemy->speed = 4;
    newEnemy->enemyID=getNumberOfEnemies();
    newEnemy->dead = 0;
	  newEnemy->firedUpon = FALSE;    
}

int setEnemyHealth(int enemyID, int newHealth)	{
	getEnemyGroup(NULL)->enemyArray[enemyID]->health = newHealth;
	return getEnemyGroup(NULL)->enemyArray[enemyID]->health;	
}                      
                      
EnemyGroup getEnemyGroup(EnemyGroup enemyList)
{
    
    static EnemyGroup e;
    
    if(enemyList != NULL) {
        e = enemyList;
    }
    
    return e;
}

void present_enemy(Display d)
{
    EnemyGroup enemyList = getEnemyGroup(NULL);
    for(int i=1; i<=enemyList->numberOfEnemies; ++i)
    {
        if(!isDead(i))
        {
            drawEnemy(d, enemyList->enemyArray[i]->x, enemyList->enemyArray[i]->y, 50/*width*/, 50/*height*/);
        }
    }
}

int getEnemyHealth(int enemyIndex)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyIndex];
    return e->health;
}

void freeEnemy(int enemyID)
{
    free(getEnemyGroup(NULL)->enemyArray[enemyID]);
}

int moveEnemy(int enemyID )
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
  if(!isDead(enemyID) ) {
    if(e->pathProgress < e->enemyPath->pathLength - e->speed) {
      e->pathProgress += e->speed;
      e->x = e->enemyPath->pathCoords[e->pathProgress][0];
      e->y = e->enemyPath->pathCoords[e->pathProgress][1];
      return 0;
    } else {
      damageHealth(e->damage);
      e->dead = 1;
      return 0;
    }
  }
  return 1;
}

int setEnemyX(int enemyID, int newX)	{

	Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	e->x = newX;
	return e->x;
}

int setEnemyY(int enemyID, int newY)	{

	Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	e->y = newY;
	return e->y;
}

BOOL getFiredUpon(int enemyID)	{

	Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	return e->firedUpon;

}

int isDead(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(e->dead == 1) {
        return 1;
    }
    else {
        return 0;
    }
}


int inRange(int tX, int tY, int tRange, int enemyID)
{
    if( enemyID > getNumberOfEnemies() )
    {
        fprintf(stderr,"ERROR: tower calling inRange on enemy that never existed");
        return -1;
    }
    
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    
    int distanceBetweenTowerAndEnemy = (int)sqrt( pow((double)(e->x-tX),2) +
                                              pow((double)(e->y-tY),2)    );
    if(distanceBetweenTowerAndEnemy<tRange){
		e->firedUpon = TRUE;
        return 1;
    }
    else {
		e->firedUpon = FALSE;
        return 0;
    }

}

void damageEnemy(int damage, int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    
    int damageToBeDone = damage - e->armour;
    if(damageToBeDone <= 0) {
      damageToBeDone = 0;
    }
    
    e->health -= damageToBeDone;
    if(e->health<=0)
    {
        e->dead=1;
        addGold(e->maxHealth);
        // drawDeath(e->x, e->y);
    }
}


void towerGetTargetPos(int * towerTargetPosition, int enemyID)
{

    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    
    towerTargetPosition[0] = e->x;
    towerTargetPosition[1] = e->y;
    
  
}


void printEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];

    printf("Enemy x = %d, enemy y = %d, enemy health = %d\n\n", e->x, e->y, e->health);

}



int **createPathArray(int rows)
{
    int columns=2;
    int **array = (int **)calloc((size_t)rows , sizeof(int *) );
    if(array==NULL)
	{
		printf("****** Path calloc 1 failed ********* \n");
		exit(1);
	}
    //changed this to allow freeing without knowing number of rows - Ben
    array[0] = (int *)calloc((size_t)(rows * columns),  sizeof(int) );
    if(array[0]==NULL)
	{
		printf("****** Path calloc 2 failed ********* \n");
		exit(1);
	}
    for(int i = 1; i < rows; i++) {
        array[i]=array[i-1]+columns;
    }
  
    return array;
}


void testEnemy()
{
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("Test_createEnemy(): Creating valid enemies");
    sput_run_test(Test_createEnemy);
    sput_leave_suite();
    sput_finish_testing();
}





/*int main()
{
    createLevelPaths();
    createEnemyGroup();
    createEnemy();
    createEnemy();
    createEnemy();
    for(int i = 0; i < 100; i++) {
      moveEnemy(3);
      printEnemy(3);
    }
}*/

