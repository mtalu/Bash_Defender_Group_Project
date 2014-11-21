#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/sput.h"
#include "../includes/tower.h"
#include "./../includes/actionQueueDataStructure.h"
#include "../includes/parser.h"
#include "../includes/main.h"
#include "../includes/enemy.h"


int main()
{	
	//testing();
    
    createPath();
    createEnemyGroup();
    createEnemy();
    createTowerGroup();
	createActionQueue();
	createGame();
	createTower();
    
    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    addGold(getGame(NULL),100);
    Display d = init_SDL();
 
    do{
        startFrame(d);
        displayInfoWindow(d);
        terminal_window(d, pass, clear,inputCommand);
        if(inputCommand)
        {
            parse(inputCommand);
        }
        updateInfoWindow(NULL);
        //parse("cat t1                                             ");
        present_enemy(d);
        present_tower(d);
        
        endFrame(d);
    } while(/*moveEnemy(1) != 1 &&*/ !terminal_window(d, pass, clear, inputCommand));
    
    //while(moveEnemy(1) != 1);
    shutSDL(d);

    return 0;
}

void testing()	{
    //	testingGameStructure();
	//testingActionQueue();
	//parseToQueueTesting();
    testEnemy();
	testingTowerModule();
	parseToTowerTesting();
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
	
    createPath();
    createTowerGroup();
	createActionQueue();
	createGame();
	createTower();
	createTower();
	addGold(getGame(NULL),10);
	addGold(getGame(NULL),10);
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
	createPath();
	createTower();
	parse("upgrade r t1");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == range, "First option in queue: range");
	parse("upgrade p t1");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == power, "Last option in queue: power");
	parse("upgrade s t1");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == speed, "Last option in queue: speed");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == range, "First option in queue: range");
	free(getQueue(NULL));
}
