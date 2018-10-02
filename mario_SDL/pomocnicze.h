#pragma once
#ifndef pomocnicze_h
#define pomocnicze_h
#include "SDL_inc.h"
#ifdef __cplusplus
extern "C"
#endif

#define BORDERWIDTHVERT 0.02
#define BORDERHEIGHT 0.1
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#include <stdio.h>

void DrawBorders(SDL_Surface* screen, SDL_Rect* borders);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
	
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

int roundValue(double value);
#endif // !pomocnicze_h
