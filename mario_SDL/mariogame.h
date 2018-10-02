
#pragma once

#include "SDL_inc.h"
#include <string.h>
#include "pomocnicze.h"
#include "mariohero.h"

#define HAMOWANIE 1
#define BIEGANIE 0
#define MARIOMOVE M_PI/4
#define MARIOSPEED 9
#define MARIOJUMPALTITUDE 6
#define MARIOJUMP M_PI/3
#define MARIOGRAV M_PI/2


class mariogame
{
public:
	mariogame();
	~mariogame();
	int initest();
	int start();


private:
	int lastSec;
	void clearApplication(mariolevel* level, mariohero* hero);
	void newGame(mariolevel* level,mariohero* hero);
	bool initialized;
	void endGame(mariolevel* level, mariohero* hero);
	void drawCommunicat(SDL_Surface* screen, const char* txt);
	int currentLevel;
	SDL_Rect borders[4];
	void initBorders();
	bool gamemenu();
	SDL_Window* window = NULL;
	//The surface contained by the window
	SDL_Surface *screen = NULL;
	SDL_Surface *charset = NULL;
	SDL_Texture *scrtex = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event event;
	bool quit = false;
	int frames, tick1, tick2, fps;
	double delta, worldtime,fpstimer, wektor_x, wektor_y;;
	char text[128];
	bool counterFPS();
	bool initWindow();

};

 // !mariogame

