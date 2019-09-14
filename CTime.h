#ifndef _CTime_h_included
#define _CTime_h_included

#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#define FPS_MAX 60

//ŽžŠÔ
typedef struct{

	Uint32 interval;
	Uint32 nowtime;
	Uint32 lasttime;
	Uint32 under;
	int isDelay;
	Uint32 clock;//milisecond.

	Uint32 fpsclock;
	Uint32 framecount;
	Uint32 fps;


}CTime;

void CTimeReset(CTime *);
void CTimeWait(CTime *);
void CTimeChangeFPS(CTime *, int);

#endif
