#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <iostream>
#include "Configuration.h"
#include "FileHandler.h"
#include "CoverFlowOnScreen.h"
#include "Helper.h"
#include <vector>
#include <unistd.h>
#include "CTime.h"
#include <time.h> 
#include "main.h"
using namespace std;

int main(int argc, char* argv[])
{
		// Set emulator
	if(argc > 1) {
		emulatorType = argv[1];
		screenType = "GAMELIST";	
	}	

	int posX;
	int posY;
	if(argc == 4) {
		posX = atof(argv[2]);
		posY = atof(argv[3]);
	}
	if(posX < 0) posX = 0;
	if(posY < 0) posY = 0;
	
	Init(posX, posY);	

	/* Thread Loop */
	CTimeReset(&gametime);
	CTimeChangeFPS(&gametime,FPS);	
	while (!quiteCoverFlow)
	{
		_CoverFlowOnScreen.GetController(event);
		if(!gametime.isDelay && !_CoverFlowOnScreen.switchEnvironment){			
			_CoverFlowOnScreen.Update();			

			currentThreadDelay--;
		}
		CTimeWait(&gametime);
			
		if(_CoverFlowOnScreen._quit) {
			quiteCoverFlow = true;
		}

		if(_CoverFlowOnScreen.switchEnvironment) {
			_CFState = _CoverFlowOnScreen._CFState;
			screenType = _CFState == CoverFlowOnScreen::EMULATOR ? "EMULATOR" : "GAMELIST";
			_CoverFlowOnScreen.switchEnvironment = false;
			_CoverFlowOnScreen.Cleanup();
			emulatorType = _CoverFlowOnScreen._cfg.emulatorType;
			Quit();
			Init(0, 0);
		}
	}

	/* Cleanup all images */
	Quit();
	return 0;	
}

void Init(int posX, int posY) {
	cout << "Loading..." << endl;

	//Reset classes
	_helper.Init();
	_config  = Configuration();		
	_fileHandler = FileHandler();
	_CoverFlowOnScreen = CoverFlowOnScreen();		
	_CoverFlowOnScreen._CFState = _CFState;

	// Set emulator
	_config._cfg.emulatorType = emulatorType;
	_config.Init();
	_helper._cfg = _config._cfg;

	if (_config._cfg.emulatorType.empty()) {
		_helper.WriteTextToLogFile("Set default emu to NES"); //log
		_config._cfg.emulatorType = "NES";
	}	

	_CFState = screenType == "EMULATOR" ? CoverFlowOnScreen::EMULATOR : CoverFlowOnScreen::GAMELIST;
	_CoverFlowOnScreen._CFState = _CFState;
	_helper.WriteTextToLogFile("Start"); //log

	/* Set CoverFlow variables */
	_helper.WriteTextToLogFile("Set coverflowvars"); //log
	_CoverFlowOnScreen._cfg = _config._cfg;
	_CoverFlowOnScreen._cfg.currentPosX = posX;
	_CoverFlowOnScreen._cfg.currentPosY = posY;

	/* Start Coverflow */
	_helper.WriteTextToLogFile("Init coverflow"); //log
	_CoverFlowOnScreen.Init();	
}

void Quit() {
	//_CoverFlowOnScreen.Cleanup();
	_helper.WriteTextToLogFile("you quit"); //log
	SDL_Quit();
	TTF_Quit();
}