#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "mariogame.h"

extern "C" {
//#ifdef BIT64
//#include"./sdl64/include/SDL.h"
//#include"./sdl64/include/SDL_main.h"
//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
//#endif
}

// main
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char **argv) {

	mariogame* game = new mariogame;
	
	game->initest();

	return 0;
	};
