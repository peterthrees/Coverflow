

#include "string.h"
#include <vector>
#include "Screen.h"


Screen::Screen(){}

void Screen::Init() { 
		/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("ConverFlow", "ConverFlow");

	// Hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
	
    TTF_Init(); 
}

void Screen::Update() { 

}

void Screen::DrawImage(SDL_Surface *imgSurface, Sint16 posX, Sint16 posY, bool freeSurface) {
	Uint16 posW = imgSurface->w;
	Uint16 posH = imgSurface->h;
	SDL_Rect rectPos = { posX, posY, posW, posH };
	SDL_BlitSurface(imgSurface, NULL, screen, &rectPos);
	if(freeSurface) SDL_FreeSurface(imgSurface);	
}

void Screen::DrawText(SDL_Surface *textSurface, Sint16 posX, Sint16 posY,  bool freeSurface) {
	Uint16 posW = textSurface->w;
	Uint16 posH = textSurface->h;
	SDL_Rect rectPos = { posX, posY, posW, posH };
    SDL_BlitSurface(textSurface, NULL, screen, &rectPos);
	if(freeSurface) SDL_FreeSurface(textSurface);   
}

void Screen::ClearSurface(SDL_Surface *clearSurface) {
	SDL_FreeSurface(clearSurface);
}

void Screen::Flip() { 
	SDL_Flip(screen);
}

const Sint16 Screen::GetScreenWidth() {
	return screen->w;
}

const Sint16 Screen::GetScreenHeight() {
	return screen->h;
}