// CoverFlowOnScreen.h
#ifndef CoverFlowOnScreen_H
#define CoverFlowOnScreen_H

#include "string.h"
#include <vector>
#include "Configuration.h"
#include "FileHandler.h"
#include "Helper.h"
#include "Screen.h"

using namespace std;

class CoverFlowOnScreen {
public:

	/* Classes */
	Helper _helper;
	Screen _screen;
	FileHandler _fileHandler;	

	/* Surfaces */	
	SDL_Surface* imgBookIcon;
	SDL_Surface* imgGameIcon;	
	SDL_Surface* imgConsoleIcon;	
	SDL_Surface* imgBoxUnknown;
	SDL_Surface* imgBackground;
	SDL_Surface* imgBar;
	SDL_Surface* imgFavIconOn;
	SDL_Surface* imgFavIconOff;
	SDL_Surface* defaultBoxArtShadLarge;
	SDL_Surface* defaultBoxArtShadSmall;
	SDL_Surface* characterSurface;
	SDL_Surface* imgMsgboxSurface;
	
	/* Buttons */
	SDL_Surface* imgButtonY;
	SDL_Surface* imgButtonX;
	SDL_Surface* imgButtonB;
	SDL_Surface* imgButtonA;
	SDL_Surface* imgButtonL;
	SDL_Surface* imgButtonR;
	SDL_Surface* imgButtonStart;
	SDL_Surface* imgButtonSelect;
	SDL_Surface* imgButtonHome;
	SDL_Surface* imgButtonUp;
	SDL_Surface* imgButtonDown;
	SDL_Surface* imgButtonLeft;
	SDL_Surface* imgButtonRight;
	SDL_Surface* imgButtonAll;

	/* Text */
	SDL_Color foregroundColor;

	SDL_Surface* textSurface;
	SDL_Surface* textButtonA;
	SDL_Surface* textButtonB;
	SDL_Surface* textButtonX;
	SDL_Surface* textButtonY;
	SDL_Surface* textButtonL;
	SDL_Surface* textButtonR;
	SDL_Surface* textButtonStart;
	SDL_Surface* textButtonSelect;
	SDL_Surface* textButtonHome;
	SDL_Surface* textButtonUp;
	SDL_Surface* textButtonDown;
	SDL_Surface* textButtonLeft;
	SDL_Surface* textButtonRight;
	SDL_Surface* textButtonAll;	
	SDL_Surface* textErrorMsg;	
	SDL_Surface* textThemeMsg;
	SDL_Surface* textCurrentThemeMsg;

	TTF_Font* fontCharacter;
	TTF_Font* fontGameList;
	TTF_Font* fontConsolename;
	TTF_Font* fontFilename;
	

	/* Other variables */
	enum coverFlowState { EMULATOR, GAMELIST };
	coverFlowState _CFState = EMULATOR;

	long deltaLastTime = 0;
	float deltaTime = 0.0;

	bool updateCharacter = true;
	bool updateText = true;	
    bool _quit = false;
    bool switchEnvironment = false;
    bool btnPressedUp = false;
    bool btnPressedDown = false;
    bool btnPressedLeft = false;
    bool btnPressedRight = false;
    bool btnPressedY = false;
    bool btnPressedX = false;
    bool btnPressedB = false;
    bool btnPressedA = false;
    bool btnPressedL = false;
    bool btnPressedR = false;
    bool btnPressedSelect = false;
    bool btnPressedStart = false;

	string _screenType = "EMULATOR";
	string _fullFilePath = ""; 

	
	struct CFgamelist 
	{
		string fileName = "";
		string boxartName = "";
		string screenshotName = "";
		SDL_Surface* boxartSurface;
		SDL_Surface* screenSurface;
		bool useDefault = false;
		bool useDefaultBoxart = false;
		bool useDefaultScreen = false;
		bool displayScreenshot = false;
		bool isLoaded = false;
		bool isFav = false;

	};


	struct CFAttributes
	{
		bool isBoxArtMovingLeft = false;
		bool isBoxArtMovingRight = false;
		bool isBoxArtSkipLeft = false;
		bool isBoxArtSkipRight = false;		
		float scrollRangeLeft = 0;
		float scrollRangeRight = 0;	
		int currentSkipImage = 0;
		bool isLoading = false;
	};	

	Configuration::CFConfig _cfg;
	CFgamelist _cfl[5000];
	CFAttributes _cfa;

	vector<FileHandler::FileContainer> _fileCollection;	
	vector<string> _characterList;
	vector<string> _favList;
	vector<string> _themeList;
	int _themePosition = 0;
	bool _showThemeScreen = false;

    CoverFlowOnScreen();

	void Init(); 
	void LoadAllFiles();
	void Load();
	void Update();
	void NextCharacter();
	void PrevCharacter();
	void UpdateFavorite();
	void LoadAssets();
	void PreLoadImages();
	void LoadImages(int currentLoadNr);
	void RefreshImages();
	void DrawBackground();
	void DrawMainBoxArt();
	void DrawLeftBoxArt();
	void DrawIcons();
	void DrawButtons();
	void DrawRightBoxArt();
	void ScrollImagesLeft();
	void ScrollImagesRight();
	void ScrollSkipImagesLeft();
	void ScrollSkipImagesRight();
	void DrawThemeScreen();
	void DrawTextError(string errorMsg);
	void ReloadCharacter(); 
	void ReloadGamePosition();
	void ReloadConsoleName();
	void ReloadText();	
	void SwitchView();	
	void findfile(string filename, string romPath);
	void RunGame();

	/* Global */
	const string GetCurrentFilename();
	const string GetCurrentCharacter();
	const string GetGameposition();

	void Cleanup();
	void GetController(SDL_Event event);
	void CheckController();



};

#endif