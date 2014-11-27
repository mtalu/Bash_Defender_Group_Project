#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../includes/sput.h"
#include "../includes/levelController.h"
#include "../includes/tower.h"
#include "./../includes/actionQueueDataStructure.h"
#include "../includes/parser.h"
#include "../includes/main.h"
#include "../includes/enemy.h"


int main()

{	

    //testing();
    initLevel();
    createEnemy();
    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    addGold(100);
    Display d = init_SDL();
    int steps=0;
    do{
        ++steps;
        startFrame(d);
        terminal_window(d, pass, clear,inputCommand);
        if(inputCommand)
        {
            parse(inputCommand);
        }
        statsMonitor();
        towerMonitor(0, NULL);
        present_enemy(d);
        present_tower(d);
        checkActQueue();
	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            int move = moveEnemy(i);
           
        }
        if(steps%100 == 0)
        {
            userCreateTower(rand()%940, rand()%780);
            createEnemy();
        }
        endFrame(d);
    } while(/*moveEnemy(1) != 1 &&*/ !terminal_window(d, pass, clear, inputCommand));
    
    //while(moveEnemy(1) != 1);
    shutSDL(d);


    return 0;
}




void testing()	{

    testingGameStructure();
    testingActionQueue();
    parseToQueueTesting();
    testEnemy();
    testingTowerModule();
    parseToTowerTesting();
    towerToEnemyTesting();

    testValidParses();


}

void towerToEnemyTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyInRange(): tower detecting single enemy in range");
	sput_run_test(testEnemyInRange);
	sput_leave_suite();

	sput_finish_testing();
}

void testEnemyInRange()	{

    createLevelPaths(); 
    createTowerGroup();
	createActionQueue();
	createGame();
	createTower();
	createEnemyGroup();
	createEnemy();
	setEnemyHealth(1,100);
	setEnemyX(1,50);
	setEnemyY(1,50);
	setTowerY(1,400);
	setTowerX(1,400);
	setTowerRange(1,10);
	sput_fail_unless(inRange(400,400,10,1)== 0, "Enemy 1 is out of range of tower 1");
	fire();
	sput_fail_unless(getEnemyHealth(1) == 100, "Out of range enemy 1 has full health after tower has fired");
	setEnemyX(1,395);
	setEnemyY(1,395);
	sput_fail_unless(inRange(400,400,10,1)== 1, "Enemy 1 is in range of tower 1");
	sput_fail_unless(getEnemyHealth(1) == 100, "Enemy 1 has full health");
	fire();
	//sput_fail_unless(getEnemyHealth(1) == 100 - getTowerDamage(1),"In range enemy has reduced health from tower damage");
	int i;
	for(i = 0; i < 11; i++)	{
	fire();
	}
	sput_fail_unless(isDead(1) == 1, "Enemy dead after being fired on 10 times");	
	
}

void parseToTowerTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testParseToTower():Tower popping parsed commands");
	sput_run_test(testParseToTower);
	sput_leave_suite();
	
	sput_finish_testing();

}

void parseToQueueTesting()	{
	
	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testValidParses():Testing Valid parsed commands being placed on queue");
	sput_run_test(testValidParses);
	sput_leave_suite();
	
	sput_finish_testing();
}

void testParseToTower()	{

	createLevelPaths();	
    createTowerGroup();
	createActionQueue();
	createGame();
	createTower();
	createTower();
	addGold(10);
	addGold(10);
	parse("upgrade r t1");
	parse("upgrade r t2");
	delayGame(ACTIONCOOLDOWN);
	sput_fail_unless(getFirstTarget() == 1, "First target is 1");
	sput_fail_unless(getLastTarget() == 2, "Last target is 2");
	createTower();
	parse("upgrade r t3");
	sput_fail_unless(getLastTarget() == 3, "Last target is 3");
	sput_fail_unless(checkActQueue()== upgrade, "Upgrade command popped by tower 1");	
	delayGame(ACTIONCOOLDOWN);
	sput_fail_unless(checkActQueue()== upgrade, "Upgrade command popped by tower 2");
	freeAllTowers();
	free(getTowerGrp(NULL));
	free(getGame(NULL));
	free(getQueue(NULL));
}

void testValidParses()	{
	
	createActionQueue();
	createTowerGroup();
	createLevelPaths();
	createTower();
    //  printf("\n135\n\n");
    sput_fail_unless(parse("upgrade r t1")== 1, "upgrade r t1 is valid command");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == range, "First option in queue: range");
    //printf("\n139\n\n");
    sput_fail_unless(parse("upgrade p t1")== 1, "upgrade p t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == power, "Last option in queue: power");
    //printf("\n143\n\n");

    sput_fail_unless(parse("upgrade s t1")== 1, "upgrade s t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == speed, "Last option in queue: speed");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == range, "First option in queue: range");
    //printf("\n149\n\n");
    sput_fail_unless(parse("  ??D--") == 0, "  ??D-- is invalid command");
    //printf("\n151\n\n");
    sput_fail_unless(parse("upgrade r r1") == 0, "upgrade r r1 is invalid command");
    //printf("\n153\n\n");
    sput_fail_unless(parse("upgrade r t") == 0, "upgrade r t is invalid command");
    //printf("\n155\n\n");
    sput_fail_unless(parse("upgrade t") == 0, "upgrade t is invalid command");
    //printf("\n157\n\n");
    sput_fail_unless(parse("cat t") == 0, "cat t is invalid command");
    //printf("\n159\n\n");




	free(getQueue(NULL));
}

