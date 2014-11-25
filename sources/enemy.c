#include <stdio.h>
#include <stdlib.h>
#include "../includes/sput.h"
#include <math.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 300

#include "../includes/enemy.h"

struct enemy {
    int x, y;
    int pathProgress;
    int health;
    int speed;
    int enemyID;
    BOOL firedUpon;
    int dead;
};

struct enemyGroup {
    int numberOfEnemies;
    Enemy * enemyArray;
};

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
    
    createPath();
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
    
    int** p = getPathPointer(NULL);
    newEnemy->pathProgress = 0;
    newEnemy->x = p[0][0];
    newEnemy->y = p[0][1];
    newEnemy->health = 1;
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
            drawEnemy(d, enemyList->enemyArray[i]->x, enemyList->enemyArray[i]->y, 100, 100);
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
    int ** path = getPathPointer(NULL);
  if(e->x < SCREEN_WIDTH - e->speed) {
    e->pathProgress += e->speed;
    e->x = path[e->pathProgress][0];
    e->y = path[e->pathProgress][1];
    return 0;
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
    
    e->health -= damage;
    if(e->health<=0)
    {
        e->dead=1;
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

void freePathArray(int **path)
{

    free(path[0]);
    free(path);
}

void layPath(int **path)
{
    for(int i = 0; i < SCREEN_WIDTH; i++) {
        path[i][0] = i;
        path[i][1] = SCREEN_HEIGHT/2;
    }
}
                                            
void createPath()
{
    int **path = createPathArray(SCREEN_WIDTH);
    layPath(path);
    getPathPointer(path);
}

int **getPathPointer(int **newPath)
{
    
    static int **path;
    
    if(newPath != NULL) {
        path = newPath;
    }
    
    return path;
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
    testEnemy();
}
*/



























