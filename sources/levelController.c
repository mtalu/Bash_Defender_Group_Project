/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers -----------*/

#include "../includes/debug.h"
#include "../includes/sput.h"
#include "../includes/levelController.h"

/*---------- Functions ----------*/
int SCREEN_WIDTH_GLOBAL;
int SCREEN_HEIGHT_GLOBAL;

struct keyword	{

	levelCommand lCommand;
	LCmdProp *propertiesList;	
	int nProperties;
	Keyword prev;

};

struct lCmdProp	{
	property p;
	int propertyValue;
};

struct keywordQueue	{

	Keyword start;
	Keyword end;
	int nCommands;
};

void printQueue()	{
	
	KeywordQueue kQueue = getKWQueue(NULL);
	Keyword curr = kQueue->start;
	int cmdN = kQueue->nCommands;
	int prop;
	printf("There are %d commands in queue\n",kQueue->nCommands);
	while(curr->prev != NULL)	{
		printf("commands %d\n",cmdN);
		printf("keyword is: %d\n",curr->lCommand);
		printf("Has %d properties\n",curr->nProperties);
		for(prop = 1; prop <= curr->nProperties; prop++)	{
			printf("property %d is %d\n",prop,curr->propertiesList[prop-1]->propertyValue);
		}	
		curr = curr->prev;
	}	
	printf("keyword is: %d\n",curr->lCommand);
	printf("Has %d properties\n",curr->nProperties);
	for(prop = 1; prop <= curr->nProperties; prop++)	{
		printf("property %d is %d\n",prop,curr->propertiesList[prop-1]->propertyValue);
	}	
}

Keyword createKeyword()	{

	Keyword newKw = (Keyword) malloc(sizeof(*newKw));
	newKw->propertiesList = NULL;
	newKw->prev = NULL;
	newKw->nProperties = 0;
	return newKw; 
}

void addKWtoQueue(Keyword nwKW)	{

		KeywordQueue kQueue = getKWQueue(NULL);
		if((kQueue->start == NULL) && (kQueue->end == NULL))	{
			printf("empty queue\n");
			kQueue->start = kQueue->end = nwKW;
		} else {
			kQueue->end->prev = nwKW;
			kQueue->end = kQueue->end->prev;
		}
		kQueue->nCommands++;

}

void createKeywordQueue()	{

	KeywordQueue newQueue = (KeywordQueue) malloc(sizeof(*newQueue));
	newQueue->start = NULL;
	newQueue->end = NULL;
	newQueue->nCommands = 0;
	getKWQueue(newQueue);
}

KeywordQueue getKWQueue(KeywordQueue kwQueue)	{

	static KeywordQueue currKWQueue;

	if(kwQueue != NULL)	{
		currKWQueue = kwQueue;
	}

	return currKWQueue;

}

/*
 *Initializes all data structures required for level
 */
void initLevel()    {
	createKeywordQueue();
	createLevel();
    //createLevelPaths();
  //  createTowerGroup();
   // createActionQueue();
    //createGame();
  //  createEnemyGroup();
//	createTowerPos();
}
/*
 *Setting This Levels variables
 */
void levelSettings()	{

	//! Positions Users can put towers
	addTowerPosNode(100,200);	
	addTowerPosNode(300,600);
	addTowerPosNode(500,300);

	//! Setting wave number
	setTotalWaveNo(3);
	
}

void createLevel()	{
	readLevelSettingsFile("../data/level1.txt");

}

void addKeyWordToken(char *token)	{
		printf("checking keyword\n");	
		Keyword newKey = createKeyword();
		if(!strcmp(token,"towerPos"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = makeTowerP;
			printf("assigned tower pos keyword\n");
		} else if(!strcmp(token,"totalWaves"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = totalWaves;
			printf("assigned total Waves keyword\n");
		} else if(!strcmp(token,"wave"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = wave;
			printf("assigned Waves keyword\n");
		} else if(!strcmp(token,"delay"))	{
			addKWtoQueue(newKey); 
			newKey->lCommand = delay;
			printf("assigned delay keyword\n");
		} else {
			fprintf(stderr,"Keyword not recognised\n");
			free(newKey);
		}
}

void addProperty(property p)	{
	KeywordQueue kWQueue = getKWQueue(NULL);
	kWQueue->end->nProperties++;

	if(kWQueue->end->nProperties == 1)	{
		kWQueue->end->propertiesList = (LCmdProp*) malloc(sizeof(*kWQueue->end->propertiesList));
	} else	{
		kWQueue->end->propertiesList = (LCmdProp*) realloc(kWQueue->end->propertiesList, (kWQueue->end->nProperties)*sizeof(*(kWQueue->end->propertiesList))); 
	}


	kWQueue->end->propertiesList[kWQueue->end->nProperties-1] = (LCmdProp) malloc(sizeof(*(kWQueue->end->propertiesList[kWQueue->end->nProperties -1])));

	kWQueue->end->propertiesList[kWQueue->end->nProperties -1]->p = p;
}



int validateLine(char *Line, int nWords)	{

	char *token;
	int wordCount = 0;
	while(wordCount < nWords)	{
		token = getToken(Line);
		sprint(token);
		if(wordCount < 1)	{
			addKeyWordToken(token); //!This is a keyword
		} else	{
			if(!checkProperty(token)){	//!This is a property
				addValue(token);		//! Not a property add a value	
			}	
		}
		free(token);
		token = NULL;
		wordCount++;
	}

	return 0;
}

int checkProperty(char *token)	{
	if(!strcmp(token,"x"))	{
		addProperty(x);
	} else if(!strcmp(token,"y"))	{
		addProperty(y);	
	} else if(!strcmp(token,"total"))	{
		addProperty(total);	
	} else if(!strcmp(token,"waveNum"))	{
		addProperty(waveID);	
	} else if(!strcmp(token,"enemeyType"))	{
		addProperty(enemyType);
	} else if(!strcmp(token,"delayTime"))	{
		addProperty(dTime);	
	} else {
		return 0;
	}

	return 1;
}

void addValue(char *token)	{
	KeywordQueue kWQueue = getKWQueue(NULL);
	int value = atoi(token);
	kWQueue->end->propertiesList[kWQueue->end->nProperties-1]->propertyValue = value;
}

char* getToken(char *line)	{
	int c = 0, letter = 0;
	char *word;
	if(line[c] == READCHAR)	{
		for(c = 0; line[c] == READCHAR || line[c] == ' '; c++){
		}
	}
	for(; line[c] != ' ' && line[c] != ENDOFSTRING; c++,letter++)	{
		word = expandCBuffer(word,letter);
		word[letter] = line[c];
		line[c] = READCHAR;		//!Marking characters as read
	}

	word[letter] = '\0';
	return word;
	

}

char* expandCBuffer(char *toExpand, int currSize)	{
	if(!currSize)	{
		return (toExpand = (char*) malloc(sizeof(char)));
	} else {
		return (toExpand = (char*) realloc(toExpand, (currSize+1)*(sizeof(char))));
	}

}

/*
 *Reads level settings file
 */
void readLevelSettingsFile(char *file)	{

	FILE *fp;
    char letter;
	int wordCount = 1;
   	char *currentLine; 
	int currSize = 0;
	if((fp = fopen(file,"r")) != NULL)	{
		while((letter = getc(fp)) != EOF)	{
			if(letter == ' ')	{
				wordCount++;
			}
				currentLine = expandCBuffer(currentLine, currSize);
			if(letter == '\n')	{
				currentLine[currSize] = ENDOFSTRING;
				validateLine(currentLine,wordCount);
				free(currentLine);
				currentLine = NULL;
				wordCount = 1;
				currSize = 0;
			} else {
				currentLine[currSize] = letter;
				currSize++;
			}
		}	
	}	
}


/*
 *Frees all Data structures
 */
void endLevel() {

    freeAllTowers();
    free(getTowerGrp(NULL));
    free(getGame(NULL));
    free(getQueue(NULL));
	freeAllTowerPositions();
	freeEnemyGroup();
	freeLevelPaths();
}

void initEnemy()  {
    //! function should be based on level and wave number as to how many enemies are created
    //
}
