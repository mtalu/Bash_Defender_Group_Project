//
//  Display.c
//
//  All functions directly related to displaying the game
//
//

#include "../includes/Display.h"
#include "../includes/parser.h"

//Window dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Information monitor dimensions
#define INFO_WINDOW_HEIGHT (SCREEN_HEIGHT / 3.5)
#define INFO_WINDOW_WIDTH (SCREEN_WIDTH / 3.5)
#define INFO_WINDOW_X (SCREEN_WIDTH - INFO_WINDOW_WIDTH)
#define INFO_WINDOW_Y (SCREEN_HEIGHT - INFO_WINDOW_HEIGHT)
#define TEXT_BORDER_X 15
#define TEXT_BORDER_Y 15

struct display {
    //Window objects
    SDL_Window  *window;
    SDL_Surface *background;
    SDL_Renderer *renderer;
    Uint32 sky, red;

    //Information monitor objects
    SDL_Surface *infoWindowSurface;
    SDL_Texture *infoWindowTexture;
    SDL_Surface *infoWindowTextSurface;
    SDL_Texture *infoWindowTextTexture;
    SDL_Rect  infoWindowRect;
    SDL_Rect  infoWindowTextureRect;
    TTF_Font *infoWindowFont;
    SDL_Color infoWindowFontColour;

    //Tower objects
    SDL_Surface *towerSurface;
    SDL_Rect     towerRect;
    SDL_Texture *towerTexture;
    
    //Enemy objects
    SDL_Surface *enemySurface;
    SDL_Rect     enemyRect;
    SDL_Texture *enemyTexture;

    SDL_Event event;
};

//Functions prototypes for functions only used internally
void initTTF(void);
TTF_Font *getInfoWindowFont(TTF_Font *font);
SDL_Surface *getInfoWindowTextSurface(char *outputString);
void crash(char *message);


Display init_SDL(){
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
    if(TTF_Init() != 0) crash("TTF_Init()");
    if(IMG_Init(0) != 0) crash("IMG_Init()");
    
    Display d = (Display)malloc(sizeof(struct display));
    d->window = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    getDisplayPointer(d);//store display ptr

    d->infoWindowSurface = IMG_Load("info_monitor.png");
    d->infoWindowTexture = SDL_CreateTextureFromSurface(d->renderer, d->infoWindowSurface);
    d->infoWindowRect = (SDL_Rect){INFO_WINDOW_X, INFO_WINDOW_Y, INFO_WINDOW_WIDTH, INFO_WINDOW_HEIGHT};
    d->infoWindowFont = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if(d->infoWindowFont == NULL) crash("TTF_(OpenFont)");
    d->infoWindowFontColour.r = 0x7c, d->infoWindowFontColour.g = 0xfc, d->infoWindowFontColour.b = 0x00;

    
    init_enemy(d, "enemy.png");
    init_tower(d, "tower.png");
    check_load_images(d->enemySurface,"info_window_background.png");
    
    TTF_Font *font;
    font = TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont() Failed: ");
    }
    
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT); //improves quality of font
    
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


//Tower and enemy graphics functions

//load an image for an enemy
void init_enemy(Display d, char *pic_name){
    d->enemySurface = IMG_Load(pic_name);
    check_load_images(d->enemySurface, pic_name);
    d->enemyTexture = SDL_CreateTextureFromSurface(d->renderer, d->enemySurface);
}

// draw an enemy at x and y coor
void drawEnemy(Display d, int x, int y, int w, int h){
    d->enemyRect = (SDL_Rect) {x, y, w, h};
    SDL_RenderCopy(d->renderer, d->enemyTexture, NULL, &(d->enemyRect));
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
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 255, 255);
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

void displayInfoWindow(Display d) {
      //Display empty information window in top left corner of main window
    SDL_RenderCopy(d->renderer, d->infoWindowTexture, NULL, &(d->infoWindowRect));
}

void updateInfoWindow( char *outputString) {
    static char * string = "            TOWER MONITOR";
    
    if(outputString) {
        string = strdup2(outputString);
    }
    
    Display d = getDisplayPointer(NULL);
    
    //Update information window with new tower information
    displayInfoWindow(d);
    
    d->infoWindowTextSurface = getInfoWindowTextSurface(string);
    d->infoWindowTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->infoWindowTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(d->infoWindowTextTexture, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect = {INFO_WINDOW_X + TEXT_BORDER_X, INFO_WINDOW_Y + TEXT_BORDER_Y, textW, textH};
    SDL_RenderCopy(d->renderer, d->infoWindowTextTexture, NULL, &dstrect);
}

void sendTextToInfoWindow(Display d, char *string) {
    //Display any text string in information window
    
    d->infoWindowTextSurface = getInfoWindowTextSurface(string);
    d->infoWindowTextTexture = SDL_CreateTextureFromSurface(d->renderer, d->infoWindowTextSurface);
    
    //Query text dimensions so text doesn't strech to whole screen
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(d->infoWindowTextTexture, NULL, NULL, &textW, &textH);

    displayInfoWindow(d);
    SDL_RenderCopy(d->renderer, d->infoWindowTextTexture,  &(d->infoWindowTextureRect) , &(d->infoWindowRect) );
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
    
    textSurface = TTF_RenderText_Blended_Wrapped(d->infoWindowFont, outputString, d->infoWindowFontColour, INFO_WINDOW_WIDTH - TEXT_BORDER_X);
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
                    printf("%s\n", pass);
                    if(strcmp(pass, clear) != 0)
                    {
                        display_text(d, pass);
                    }
                    // inputCommand = strdup2(pass);
                    
                    pass2 = pass + 2;
                    printf("testing %s", pass2);
                    parse(pass2);
                    
                    strcpy(pass, clear);
                }
                {
                    switch(d->event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done = 1;
                            break;
                    }
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
    SDL_Surface *text;
    SDL_Texture *newtexture;
    SDL_Color text_color = {255, 0, 0};
    dstrect.x = 50;
    dstrect.y = 450;
    
    text = TTF_RenderText_Solid(getInfoWindowFont(NULL), pass, text_color);
    newtexture = SDL_CreateTextureFromSurface(d->renderer, text);
    //SDL_LockTexture(newtexture, NULL, text->pixels, &text->pitch);
    if(newtexture == NULL)
    {
        printf("Panic\n");	
    }
    dstrect.w = text->w;
    dstrect.h = text->h;
    SDL_FreeSurface(text);
    SDL_QueryTexture(newtexture, NULL, NULL, &texW, &texH);
    SDL_RenderCopy(d->renderer, newtexture, NULL, &dstrect);
}

/* duplicates a string */
char *strdup2(char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}
//End of terminal functions

