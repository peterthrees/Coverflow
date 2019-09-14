// filehandler.h
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>

using namespace std;

class Configuration {
public:

//ArraySIze
	struct CFConfig
	{
		string theme = "default";

		string emulatorType = "";
		string emulatorPath = "";
		string emulatorExec = "";
		string emulatorParams = "";
		string emulatorExtensions = "*";
		int logging = 0;
		int switchToEmu = 1;

		string romPath = "";	
		string boxartPath = "";	
		string screenshotsPath = "";		
		string favSave = "";

		/* Images */
		//Console
		string imgConsoleBackground = "";
		string imgConsoleBar = "";
		string imgConsolePanel = "";
		//Emulator
		string imgBackground = ""; //background image
		string imgDefaultBoxart = "";
		string imgShadow = "";
		string imgBookIcon = "";
		string imgGameIcon = "";
		string imgConsoleIcon = "";
		string imgFavOff = "";
		string imgFavOn = "";		
		string imgUrlButtonY = "";
		string imgUrlButtonX = "";
		string imgUrlButtonB = "";
		string imgUrlButtonA = "";
		string imgUrlButtonL = "";
		string imgUrlButtonR = "";
		string imgUrlButtonStart = "";
		string imgUrlButtonSelect = "";
		string imgUrlButtonHome = "";
		string imgUrlButtonUp = "";
		string imgUrlButtonDown = "";
		string imgUrlButtonLeft = "";
		string imgUrlButtonRight = "";
		string imgUrlButtonAll = "";

		/* Fonts */
		//Console
		string fontConsole = "";
		//Emulator
		string fontCharacter = "";
		string fontGameList = "";
		string fontFilename = "";
		string fontConsolename = "";		

	    int arraySize = 5; //Ammount been displayed on screen?
	    int currentPos=0; //should be removed
	    float scrollBoxartSpeed = 400; //normal scroll speed left & right
	    float scrollScreenSpeed = 700; //normal scroll speed left & right
	    float scrollSpeedFast = 1200; //fast scroll speed L & R button
	    int skipImages = 10; //fast scroll speed skip x boxarts
	    int currentPosX=0; //Position which boxart you are
	    int currentPosY=0; //Position which character you are
	    int gamelistSize = -1; //This isn't config, but is been used to keep the count of how many games there are for your current character
	    int loadMaxGames = 5; //Amount of games preloaded on each character switch (rest will be loaded by a thread.
	    int loadNr = 0; //load game x by thread
	    bool showBoxarts = true; //when this is disabled, screenshots instead would be loaded
	    float boxartZoomStrX = 65;
	    float boxartZoomStrY = 90;	    
	    float boxartZoomStrSmallX = 43;
	    float boxartZoomStrSmallY = 60;
	    int bortartOffsetY = 0;
	    int bortartOffsetSmallY = 0;
	    int bortartFilenamePosY = 175;
	    int boxartDistance = 80; //distance between boxarts
	    float screenZoomStrX = 170;
	    float screenZoomStrY = 127;
	    float screenZoomStrSmallX = 100;
	    float screenZoomStrSmallY = 75;
		int screenFilenamePosY = 190;
	    int screenOffsetY = -7;
	    int screenOffsetSmallY = -7;	
	    int screenDistance = 160;	
	};

	CFConfig _cfg;	

	Configuration();

	void Init(); 

	void AddConfigVariable(auto name, auto value);

};

#endif