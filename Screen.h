// Screen.h
#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "string.h"
#include <sstream>
#include <vector>
#include <algorithm>



using namespace std;

class Screen {
	public:

	SDL_Surface *screen;

	Screen();
	void Init();
	void Update();
	void DrawImage(SDL_Surface *imgSurface, Sint16 posX, Sint16 posY, bool freeSurface);
	void DrawText(SDL_Surface *textSurface, Sint16 posX, Sint16 posY, bool freeSurface);
	void ClearSurface(SDL_Surface *clearSurface);
	void Flip();
	const Sint16 GetScreenWidth();
	const Sint16 GetScreenHeight();

};

#endif