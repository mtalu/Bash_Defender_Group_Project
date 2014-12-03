//
//  Display.c
//
//  All functions directly related to displaying the game
//
//

#include "../includes/Display.h"
#include "../includes/parser.h"
#include "../includes/tower.h"
#include <stdbool.h>

#define HEALTH 80

struct display {
    //Window objects
    SDL_Window  *window;
    SDL_Surface *background;
    SDL_Renderer *renderer;
    Uint32 sky, red;

    //Tower monitor objects
    SDL_Surface *infoWindowSurface;
    SDL_Texture *infoWindowTexture;
    SDL_Surface *infoWindowTextSurface;
    SDL_Texture *infoWindowTextTexture;
    SDL_Rect  infoWindowRect;
    SDL_Rect  infoWindowTextureRect;
    TTF_Font *infoWindowFont;
    SDL_Color infoWindowFontColour;
    
    //Stats monitor objects
    SDL_Surface *statsMonitor;
    SDL_Texture *statsMonitorTexture;
    SDL_Surface *statsMonitorTextSurface;
    SDL_Texture *statsMonitorTextTexture;
    SDL_Rect statsMonitorRect;
    SDL_Rect statsMonitorTextureRect;

    //Tower objects
    SDL_Surface *towerSurface;
    SDL_Rect     towerRect;
    SDL_Texture *towerTexture;
    
    //Enemy objects
    SDL_Surface *enemySurface;
    SDL_Rect     enemyRect;
    SDL_Texture *enemyTexture;

    //enemy health
    SDL_Rect healthBar;

    SDL_Event event;
};

//Functions prototypes for functions only used internally
void initTTF(void);
TTF_Font *getInfoWindowFont(TTF_Font *font);
SDL_Surface *getInfoWindowTextSurface(char *outputString);
void crash(char *message);

void getWindowSize(int *w, int *h){
    Display d = getDisplayPointer(NULL);
    SDL_GetWindowSize(d->window, w, h);
}

Display init_SDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
    if(TTF_Init() != 0) crash("TTF_Init()");
    if(IMG_Init(0) != 0) crash("IMG_Init()");
    
    Display d = (Display)malloc(sizeof(struct display));
    d->window = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_YN);
    getDisplayPointer(d);

    getWindowSize(&SCREEN_WIDTH_GLOBAL,&SCREEN_HEIGHT_GLOBAL);
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    

    d->infoWindowSurface = IMG_Load("info_monitor.png");
    d->infoWindowTexture = SDL_CreateTextureFromSurface(d->renderer, d->infoWindowSurface);
    d->infoWindowRect = (SDL_Rect){TOWER_MONITOR_X, TOWER_MONITOR_Y, TOWER_MONITOR_WIDTH, TOWER_MONITOR_HEIGHT};
    d->infoWindowTextureRect = (SDL_Rect) {TOWER_MONITOR_X + TOWER_TEXT_BORDER_X, TOWER_MONITOR_Y + TOWER_TEXT_BORDER_Y, 0, 0};

    d->infoWindowFont = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if(d->infoWindowFont == NULL) crash("TTF_(OpenFont)");
    d->infoWindowFontColour.r = 0x7c, d->infoWindowFontColour.g = 0xfc, d->infoWindowFontColour.b = 0x00;
    
    d->statsMonitor = IMG_Load("stats_monitor.png");
    d->statsMonitorTexture = SDL_CreateTextureFromSurface(d->renderer, d->statsMonitor);
    d->statsMonitorRect = (SDL_Rect) {STATS_MONITOR_X, STATS_MONITOR_Y, STATS_MONITOR_WIDTH, STATS_MONITOR_HEIGHT};
    d->statsMonitorTextureRect = (SDL_Rect) {STATS_MONITOR_X + STATS_MONITOR_BORDER_X, STATS_MONITOR_Y + STATS_MONITOR_BORDER_Y, 0, 0};

    

    putenv("SDL_VIDEODRIVER=dga");

    
    init_enemy(d, "sdl2-spritesheet-actual.png");
    init_tower(d, "tower.png");
    
    check_load_images(d->infoWindowSurface,"info_window_background.png");

    getDisplayPointer(d);//store display ptr
    
    TTF_Font *font;
    font = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont() Failed: ");
    }
    //improves quality of font
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    
    getInfoWindowFont(font);//stores font ptr


    return d;
}

Display getDisplayPointer(Display d)
{
    
	static Display disp;
    
	if(d != NULL)	{
		disp = d;
	}
    
	return disp;
}

void crash(char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    SDL_Quit();
}



//damage line from X & Y to target X Y>
void drawLine(Display d, int X_from, int Y_from, int X_target, int Y_target){
    SDL_SetRenderDrawColor(d->renderer, 252, 1, 1, 255);
    SDL_RenderDrawLine(d->renderer, X_from, Y_from, X_target, Y_target);
}

//Tower and enemy graphics functions

//load an image for an enemy
void init_enemy(Display d, char *pic_name){
    d->enemySurface = IMG_Load(pic_name);
    check_load_images(d->enemySurface, pic_name);
    d->enemyTexture = SDL_CreateTextureFromSurface(d->renderer, d->enemySurface);
}


void init_picture(SDL_Renderer *renderer, SDL_Surface * surface, SDL_Texture *texture, char *pic_name){
    surface = IMG_Load(pic_name);
    check_load_images(surface, pic_name);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}


// draw an enemy at x and y coor
void drawEnemy(Display d, int x, int y, int w, int h, double currentHealth, double maxHealth){
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 100) % 4;
    SDL_Rect srcrect = { sprite * w, 0, w, h};
    d->enemyRect = (SDL_Rect) {x, y, w, h};
    SDL_RenderCopy(d->renderer, d->enemyTexture, &srcrect, &d->enemyRect);
    
    double color = (255*((double)currentHealth/maxHealth));
    printf("%f\n", color);
    SDL_SetRenderDrawColor(d->renderer, 0, color, 0, 255);
    double health = ((double)(currentHealth * HEALTH)/maxHealth);
   // printf("%f\n", health);
    d->healthBar = (SDL_Rect) {x, y -20, health, 20};
    SDL_RenderCopy(d->renderer, d->enemyTexture, &srcrect, &d->enemyRect);
    SDL_RenderFillRect(d->renderer, &d->healthBar);
}

//loads an image for a tower
void init_tower(Display d, char *pic_name){
    d->towerSurface = IMG_Load(pic_name);
    check_load_images(d->towerSurface, pic_name);
    d->towerTexture = SDL_CreateTextureFromSurface(d->renderer, d->towerSurface);
 }

// draws the tower at x and y coor
void drawTower(Display d, int x, int y, int w, int h){
    d->towerRect = (SDL_Rect) {x, y ,w, h};
    SDL_RenderCopy(d->renderer, d->towerTexture, NULL, &(d->towerRect));
}

void startFrame(Display d) {
    //Display d = getDisplayPointer(NULL);

    SDL_SetRenderDrawColor(d->renderer, 168, 230, 255, 255);
    SDL_RenderClear(d->renderer);
}

void endFrame(Display d) {
    SDL_RenderPresent(d->renderer);
    SDL_Delay(20);
}

// check wheter image was loaded successfully
void check_load_images(SDL_Surface *surface, char *pic_name){
    if(surface == NULL){
        printf("Cannot find %s\n", pic_name);
        SDL_Quit();
        exit(1);
    }
}

//
void shutSDL(Display d){
    SDL_DestroyWindow(d->window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

//End of tower and enemy graphics functions



//Information window functions

void displayTowerMonitor() {
      //Display empty tower monitor in bottom right corner of screen
    
    Display d = getDisplayPointer(NULL);
    
    SDL_RenderCopy(d->renderer, d->infoWindowTexture, NULL, &(d->infoWindowRect));
}

void displayStatsMonitor() {
    //Display empty stats monitor in top left corner of screen
    
    Display d = getDisplayPointer(NULL);
    
    SDL_RenderCopy(d->renderer, d->statsMonitorTexture, NULL, &(d->statsMonitorRect));
}

void updateTowerMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    displayTowerMonitor();
    
    d->infoWindowTextSurface = getInfoWindowTextSurface(outputString);
    d->infoWindowTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->infoWindowTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(d->infoWindowTextTexture, NULL, NULL, &textW, &textH);
    d->infoWindowTextureRect.w = textW;
    d->infoWindowTextureRect.h = textH;
    
    SDL_RenderCopy(d->renderer, d->infoWindowTextTexture, NULL, &(d->infoWindowTextureRect));
    
    SDL_FreeSurface(d->infoWindowTextSurface);
    SDL_DestroyTexture(d->infoWindowTextTexture);
    
}

/*void testStatsMonitor (char *outputString) {
    Display d = getDisplayPointer(d);
    
    
}*/


void updateStatsMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    displayStatsMonitor();
    
    d->statsMonitorTextSurface = getInfoWindowTextSurface(outputString);
    d->statsMonitorTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->statsMonitorTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(d->statsMonitorTextTexture, NULL, NULL, &textW, &textH);
    d->statsMonitorTextureRect.w = textW;
    d->statsMonitorTextureRect.h = textH;
    
    SDL_RenderCopy(d->renderer, d->statsMonitorTextTexture, NULL, &(d->statsMonitorTextureRect));
    
    SDL_FreeSurface(d->statsMonitorTextSurface);
    SDL_DestroyTexture(d->statsMonitorTextTexture);
    free(outputString);
    
}

TTF_Font *getInfoWindowFont(TTF_Font *font)
{
    //Load font to be used in information window
    
    static TTF_Font *storedFont;
	if(font != NULL)	{
		storedFont = font;
	}

    return storedFont;
}


SDL_Surface *getInfoWindowTextSurface(char *outputString) {
    //Create text surface to be displayed in information window
    
    Display d = getDisplayPointer(NULL);
    
    SDL_Surface *textSurface;
    //TTF_Font *font = getInfoWindowFont(NULL);
    //    SDL_Color fontColour = { 0xFF, 0xFF, 0xFF };

    textSurface = TTF_RenderText_Blended_Wrapped(d->infoWindowFont, outputString, d->infoWindowFontColour, TOWER_MONITOR_WIDTH - TOWER_TEXT_BORDER_X);
    if(textSurface == NULL) crash("getInfoWindowTextSurface()");
    
    return textSurface;
}
//End of information window functions


//Terminal functions
int processEvents(Display d){

    int done = 0;
    
    while(SDL_PollEvent(&d->event))
    {
        switch(d->event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(d->window)
                {
                    SDL_DestroyWindow(d->window);
                    d->window = NULL;
                    done = 1;
                }
            }
                break;
            case
            SDL_KEYDOWN:
            {
                switch(d->event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
                break;
            case SDL_QUIT:
                //quit out of the game
                done = 1;
                break;
        }
    }
    return done;
}


int terminal_window(Display d, char *pass, char *clear, char * inputCommand)
{
    //inputCommand=NULL;
    int done = 0;
    char *pass2;
    
    display_text(d, pass);
    
    int check = 0;
    SDL_Event *event = &d->event;
    check = (SDL_PollEvent(event));
    if(check != 0)
    {
        switch (event->type)
        {
            case SDL_TEXTINPUT:
            {
                strcat(pass, event->text.text);
                display_text(d, pass);
                break;
            }
            case SDL_KEYDOWN:
            {
                if(event->key.keysym.sym == SDLK_RETURN)
                {
                    if(strcmp(pass, clear) != 0)
                    {
                        display_text(d, pass);
                    }
                    // inputCommand = strdup2(pass);
                    
                    pass2 = pass + 2;
                    parse(pass2);
                    
                    strcpy(pass, clear);
                }
				if(event->key.keysym.sym == SDLK_BACKSPACE)
				{
					if(pass[strlen(pass) - 1] != '>')
					{
						pass[strlen(pass) - 1] = '\0';
					}
					display_text(d, pass);
				}
                switch(d->event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
                	done = 1;
                	break;
                
                }
                break;
            }
                //            case SDL_TEXTEDITING:
                //            {
                //                strcpy(event->edit.text, pass);
                //                event->edit.start = pass;
                //                event->edit.length = 32;
                //            }
                //default: printf("Event code = %d\n", event->type);
        }
    }
    return done;
}

void display_text(Display d, char *pass)
{
    int texW = 200, texH = 50;
    SDL_Rect dstrect = { 5, 5, texW, texH };
	SDL_Rect imagerect = {TERMINAL_WINDOW_X, TERMINAL_WINDOW_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT};
    SDL_Surface *text, *imagesurface;
    SDL_Texture *newtexture, *imagetexture;
    SDL_Color text_color = {0, 255, 0};
    dstrect.x = TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10);
    dstrect.y = TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7));
    
	imagesurface = IMG_Load("terminalwindow.png");
	imagetexture = SDL_CreateTextureFromSurface(d->renderer, imagesurface);
    text = TTF_RenderText_Solid(d->infoWindowFont, pass, text_color);
    newtexture = SDL_CreateTextureFromSurface(d->renderer, text);
    if(newtexture == NULL)
    {
        printf("Panic\n");	
    }
    dstrect.w = text->w;
    dstrect.h = text->h;
    SDL_FreeSurface(text);
    SDL_QueryTexture(newtexture, NULL, NULL, &texW, &texH);
	SDL_RenderCopy(d->renderer, imagetexture, NULL, &imagerect);
    SDL_RenderCopy(d->renderer, newtexture, NULL, &dstrect);
	SDL_DestroyTexture(newtexture);
	SDL_DestroyTexture(imagetexture);
}

/* duplicates a string */
char *strdup2(char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}
//End of terminal functions

