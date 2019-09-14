 
//CoverFlowOnScreen.cpp
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "CoverFlowOnScreen.h"
#include <dirent.h> 
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "Helper.h"
#include <algorithm>



using namespace std;

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240


CoverFlowOnScreen::CoverFlowOnScreen() { 
	
}

void CoverFlowOnScreen::Init() {
	
	LoadAllFiles(); 

	_screen.Init();	

	Load();

}

void CoverFlowOnScreen::LoadAllFiles() {	
	_fileHandler._cfg = _cfg;	
	_fileHandler._helper = _helper;
	
	if(_CFState == EMULATOR) {
		_fileHandler.LoadEmulators();
		_fileHandler.SetCharacterEmulatorList();
		_cfg.boxartDistance = 100;
	}
	else {
		_fileHandler.LoadFavList();
		_fileHandler.LoadFiles();
		_themeList = _fileHandler.GetThemes();
		_fileHandler.SetCharacterList();
		_favList = _fileHandler._favList;
	}	
	_fileCollection = _fileHandler._fileCollection;
	_characterList = _fileHandler._characterList;
}

void CoverFlowOnScreen::Load() {	
	
	LoadAssets();	
	
	if(_fileCollection.size()>0) {
		PreLoadImages();	
	}
	
	RefreshImages();	

}

void CoverFlowOnScreen::Update() {

	long deltaNowTime = SDL_GetTicks();
	
	if (deltaNowTime > deltaLastTime) {
		deltaTime = ((float)(deltaNowTime - deltaLastTime)) / 1000;
		deltaLastTime = deltaNowTime;
	}

	if(_fileCollection.size()>0) {
		/* Update images on screen */
		ScrollImagesLeft();
		ScrollImagesRight();
		ScrollSkipImagesLeft();
		ScrollSkipImagesRight();
	}	

	/* Update the text */
	updateCharacter = false;	
	RefreshImages();
	
	/* Flip the whole screen */
	_screen.Flip();

}


void CoverFlowOnScreen::NextCharacter() {
	
	if(_cfg.currentPosY < _characterList.size()-1) {
		_cfa.isLoading = true;
		_cfg.currentPosY++;
		_cfg.currentPosX = 0;
		PreLoadImages();
		RefreshImages();
		updateCharacter = true;
		updateText = true;
		_cfa.isLoading = false;
	}

}
void CoverFlowOnScreen::PrevCharacter() {

	if(_cfg.currentPosY > 0) {
		_cfa.isLoading = true;
		_cfg.currentPosY--;
		_cfg.currentPosX = 0;
		PreLoadImages();
		RefreshImages();
		updateCharacter = true;
		updateText = true;
		_cfa.isLoading = false;
	}	
}

void CoverFlowOnScreen::UpdateFavorite() {

	/* Don't mark fav when boxart are moving */
	if(!_cfa.isBoxArtMovingLeft && !_cfa.isBoxArtMovingRight && !_cfa.isBoxArtSkipLeft && !_cfa.isBoxArtSkipRight) {
		_cfa.isLoading = true;

		bool favAction = !_cfl[_cfg.currentPosX].isFav;

		auto itr = find(_favList.begin(), _favList.end(), _cfl[_cfg.currentPosX].fileName);
		if(favAction) {
			if (itr == _favList.end()) _favList.push_back(_cfl[_cfg.currentPosX].fileName);
		}
		else {		
			if (itr != _favList.end()) _favList.erase(itr);
		}

		_cfl[_cfg.currentPosX].isFav = favAction;

		_helper.WriteVectorToFile(_cfg.favSave, _favList);

		if(_characterList[_cfg.currentPosY][0] == '*') {
			if(_favList.size() <= _cfg.currentPosX) _cfg.currentPosX = _favList.size()-1;
			PreLoadImages();
		}

		RefreshImages();
		updateCharacter = true;
		updateText = true;
		_cfa.isLoading = false;
	}
}


/**************/
/* IMAGE PART */
/**************/

void CoverFlowOnScreen::LoadAssets() {

	/* Load background */
	string tmpBackgroundUrl = _CFState == EMULATOR ? _cfg.imgConsoleBackground : _cfg.imgBackground;
	imgBackground = IMG_Load(tmpBackgroundUrl.c_str());

	/* BookIcon */
	imgBookIcon = IMG_Load(_cfg.imgBookIcon.c_str());

	/* GameIcon */
	imgGameIcon = IMG_Load(_cfg.imgGameIcon.c_str());

	/* consoleIcon */
	imgConsoleIcon = IMG_Load(_cfg.imgConsoleIcon.c_str());	

	/* Unknown game  */
	imgBoxUnknown = IMG_Load((_cfg.imgDefaultBoxart).c_str());	

	/* Fav icons */
	SDL_Surface *imgDefaultfavIconOff = IMG_Load((_cfg.imgFavOff).c_str());
	SDL_Surface *imgDefaultfavIconOn = IMG_Load((_cfg.imgFavOn).c_str());

	imgFavIconOff = SDL_DisplayFormatAlpha(imgDefaultfavIconOff);
	imgFavIconOn = SDL_DisplayFormatAlpha(imgDefaultfavIconOn);	

	/* Buttons */
	SDL_Surface *imgPreButtonY = IMG_Load((_cfg.imgUrlButtonY).c_str());
	SDL_Surface *imgPreButtonX = IMG_Load((_cfg.imgUrlButtonX).c_str());
	SDL_Surface *imgPreButtonB = IMG_Load((_cfg.imgUrlButtonB).c_str());
	SDL_Surface *imgPreButtonA = IMG_Load((_cfg.imgUrlButtonA).c_str());
	SDL_Surface *imgPreButtonL = IMG_Load((_cfg.imgUrlButtonL).c_str());
	SDL_Surface *imgPreButtonR = IMG_Load((_cfg.imgUrlButtonR).c_str());
	SDL_Surface *imgPreButtonStart = IMG_Load((_cfg.imgUrlButtonStart).c_str());
	SDL_Surface *imgPreButtonSelect = IMG_Load((_cfg.imgUrlButtonSelect).c_str());
	SDL_Surface *imgPreButtonHome = IMG_Load((_cfg.imgUrlButtonHome).c_str());
	SDL_Surface *imgPreButtonUp = IMG_Load((_cfg.imgUrlButtonUp).c_str());
	SDL_Surface *imgPreButtonDown = IMG_Load((_cfg.imgUrlButtonDown).c_str());
	SDL_Surface *imgPreButtonLeft = IMG_Load((_cfg.imgUrlButtonLeft).c_str());
	SDL_Surface *imgPreButtonRight = IMG_Load((_cfg.imgUrlButtonRight).c_str());
	SDL_Surface *imgPreButtonAll = IMG_Load((_cfg.imgUrlButtonAll).c_str());

	imgButtonY = SDL_DisplayFormatAlpha(imgPreButtonY);
	imgButtonX = SDL_DisplayFormatAlpha(imgPreButtonX);
	imgButtonB = SDL_DisplayFormatAlpha(imgPreButtonB);
	imgButtonA = SDL_DisplayFormatAlpha(imgPreButtonA);
	imgButtonL = SDL_DisplayFormatAlpha(imgPreButtonL);
	imgButtonR = SDL_DisplayFormatAlpha(imgPreButtonR);
	imgButtonStart = SDL_DisplayFormatAlpha(imgPreButtonStart);
	imgButtonSelect = SDL_DisplayFormatAlpha(imgPreButtonSelect);
	imgButtonHome = SDL_DisplayFormatAlpha(imgPreButtonHome);
	imgButtonUp = SDL_DisplayFormatAlpha(imgPreButtonUp);
	imgButtonDown = SDL_DisplayFormatAlpha(imgPreButtonDown);
	imgButtonLeft = SDL_DisplayFormatAlpha(imgPreButtonLeft);
	imgButtonRight = SDL_DisplayFormatAlpha(imgPreButtonRight);
	imgButtonAll = SDL_DisplayFormatAlpha(imgPreButtonAll);

	/* Box shadow */
	SDL_Surface *defaultBoxArtLarge = IMG_Load((_cfg.imgShadow).c_str());
	SDL_Surface *defaultBoxArtSmall = IMG_Load((_cfg.imgShadow).c_str());

	// Calculate zoomfactor
	float zoomSurfaceCenterX = 0;
	float zoomSurfaceCenterY = 0;
	float zoomSurfaceSmallX= 0;
	float zoomSurfaceSmallY= 0;	

	
	zoomSurfaceCenterX = _cfg.showBoxarts ? (_cfg.boxartZoomStrX/defaultBoxArtLarge->w) : (_cfg.screenZoomStrX/defaultBoxArtLarge->w);
	zoomSurfaceCenterY = _cfg.showBoxarts ? (_cfg.boxartZoomStrY/defaultBoxArtLarge->h) : (_cfg.screenZoomStrY/defaultBoxArtLarge->h);
	zoomSurfaceSmallX = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallX/defaultBoxArtLarge->w) : (_cfg.screenZoomStrSmallX/defaultBoxArtLarge->w);
	zoomSurfaceSmallY = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallY/defaultBoxArtLarge->h) : (_cfg.screenZoomStrSmallY/defaultBoxArtLarge->h);

	// Scale & Store
	SDL_Surface *tmpSL=NULL;
	tmpSL=zoomSurface(defaultBoxArtLarge,zoomSurfaceCenterX,zoomSurfaceCenterY,0);  // scale the image x0.5 when loaded into memory
	defaultBoxArtLarge=tmpSL;
	SDL_Surface *tmpSS=NULL;
	tmpSS=zoomSurface(defaultBoxArtSmall,zoomSurfaceSmallX,zoomSurfaceSmallY,0);  // scale the image x0.5 when loaded into memory
	defaultBoxArtSmall=tmpSS;	

	defaultBoxArtShadLarge = SDL_DisplayFormatAlpha(defaultBoxArtLarge);
	defaultBoxArtShadSmall = SDL_DisplayFormatAlpha(defaultBoxArtSmall);
	
	/* Loqd Text */
	fontCharacter = TTF_OpenFont(_cfg.fontCharacter.c_str(), 11);
    fontGameList  = TTF_OpenFont(_cfg.fontGameList.c_str(), 11);
    fontFilename = TTF_OpenFont(_cfg.fontFilename.c_str(), 11);
    fontConsolename = TTF_OpenFont(_cfg.fontConsolename.c_str(), 10);

    foregroundColor = { 255, 255, 255 };

    TTF_SetFontStyle(fontCharacter, TTF_STYLE_BOLD);
    TTF_SetFontStyle(fontGameList, TTF_STYLE_BOLD);
    TTF_SetFontStyle(fontFilename, TTF_STYLE_BOLD);
    TTF_SetFontStyle(fontConsolename, TTF_STYLE_BOLD);

    /* Fixed Text */
    textButtonY = TTF_RenderText_Solid(fontFilename, "fav", foregroundColor);
    textButtonX = TTF_RenderText_Solid(fontFilename, "switch", foregroundColor);
    textButtonA = TTF_RenderText_Solid(fontFilename, _CFState == EMULATOR ? "load" : "play", foregroundColor);
    textButtonB = TTF_RenderText_Solid(fontFilename, "emu", foregroundColor);
    textButtonSelect = TTF_RenderText_Solid(fontFilename, "quit", foregroundColor);
    textButtonStart = TTF_RenderText_Solid(fontFilename, "theme", foregroundColor);

}


void CoverFlowOnScreen::PreLoadImages() {
	/* Clear gamelist */
	fill_n(_cfl, 2000, CFgamelist{}); 
	/* Reload images in scope */
	int currentArrayPosition = 0;
	/* Fav */
	bool isFav = false;
	/* Which character? */
	char currentCharacter = _characterList[_cfg.currentPosY][0];


		/* If no fav, skip fav list */
	if(currentCharacter == '*' && _favList.size() == 0) {
		_cfg.currentPosY++;
		currentCharacter = _characterList[_cfg.currentPosY][0];
		_cfg.currentPosX = 0;
	}	

	/* Check which images we want to load (based on your current position */
	int fromPosX = 0;
	int untilPosX = 0;
	fromPosX = _cfg.currentPosX - 3;
	untilPosX = _cfg.currentPosX + 3;

	/* Preload only the first x */
	for(int i = 0; i < _fileCollection.size(); i++) {
		string character = _fileCollection[i].character;
		string fname = _fileCollection[i].fileName;
		string bxName = _fileCollection[i].boxartName;
		string scName = _fileCollection[i].screenshotName;

		if(character[0] == currentCharacter || currentCharacter == '*') {

			/* Favlist */
			isFav = false;
			if (find(_favList.begin(), _favList.end(), fname) != _favList.end())
			{
				isFav = true;
			}	
			if(currentCharacter != '*' || isFav) {
				//Filename
				_cfl[currentArrayPosition].fileName = fname;
				_cfl[currentArrayPosition].boxartName = bxName;
				_cfl[currentArrayPosition].screenshotName = scName;
				_cfl[currentArrayPosition].isFav = isFav;			
				/* Preload x games, load the other ones later by thread */
				if(currentArrayPosition >= fromPosX && currentArrayPosition <= untilPosX) {
					//BoxArt
					if((_fileCollection[i].useDefault && _cfg.showBoxarts) || (_fileCollection[i].useDefaultScreen && !_cfg.showBoxarts)) {
						_cfl[currentArrayPosition].boxartSurface = imgBoxUnknown;
						_cfl[currentArrayPosition].useDefault = true;
						_cfl[currentArrayPosition].useDefaultBoxart = _fileCollection[i].useDefault;
						_cfl[currentArrayPosition].useDefaultScreen = _fileCollection[i].useDefaultScreen;
					}
					else {
						SDL_Surface *boxartImg;
						if(_cfg.showBoxarts) boxartImg = IMG_Load(bxName.c_str());
						else boxartImg = IMG_Load(scName.c_str());
						_cfl[currentArrayPosition].boxartSurface = boxartImg;
						_cfl[currentArrayPosition].isLoaded = true;
					}					
				}
				else {
					_cfl[currentArrayPosition].useDefault = true;
					_cfl[currentArrayPosition].useDefaultBoxart = _fileCollection[i].useDefault;
					_cfl[currentArrayPosition].useDefaultScreen = _fileCollection[i].useDefaultScreen;					
					_cfl[currentArrayPosition].boxartSurface = imgBoxUnknown;			
				}

				/* Next row */
				currentArrayPosition++;
			}
		}
	} 

	/* To know how many games are loaded for current charachter */
	_cfg.gamelistSize  = currentArrayPosition-1; // -1  because array starts from 0 

	/* Thread should start loading boxarts from this nr */
	_cfg.loadNr = (_cfg.loadMaxGames-1); // -1  because array starts from 0 

}

void CoverFlowOnScreen::LoadImages(int currentLoadNr) {
	if(currentLoadNr >= 0 && currentLoadNr <= _cfg.gamelistSize) {
		string bxName = _cfl[currentLoadNr].boxartName;
		string scName = _cfl[currentLoadNr].screenshotName;

		if((_cfl[currentLoadNr].useDefaultBoxart && _cfg.showBoxarts) || (_cfl[currentLoadNr].useDefaultScreen && !_cfg.showBoxarts)) {
			_cfl[currentLoadNr].boxartSurface = imgBoxUnknown;
		}
		else {
			SDL_Surface *boxartImg;
			boxartImg = _cfg.showBoxarts ? IMG_Load(bxName.c_str()) : IMG_Load(scName.c_str());
			_cfl[currentLoadNr].boxartSurface = boxartImg;
			_cfl[currentLoadNr].isLoaded = true;
		}

		
	}
}

void CoverFlowOnScreen::RefreshImages() {
	DrawBackground();
	if(!_showThemeScreen) {
		if(_fileCollection.size()>0) {
			DrawLeftBoxArt();	
			DrawRightBoxArt();
			DrawMainBoxArt();	
			ReloadCharacter();
			ReloadGamePosition();
			ReloadText();	
		}
		else {
			DrawTextError("No roms found for: " + _cfg.emulatorType);
		}	
		DrawIcons();
		ReloadConsoleName();
		DrawButtons();	
	}
	else {
		DrawThemeScreen();
	}
}

void CoverFlowOnScreen::DrawBackground() {
	/* Load background */	
	_screen.DrawImage(imgBackground, 0, 0, false);
}

void CoverFlowOnScreen::DrawMainBoxArt() {

	/* Draw Main image */
	int OffsetY = _cfg.showBoxarts ? _cfg.bortartOffsetY : _cfg.screenOffsetY;

	SDL_Surface *mainBoxArt = _cfl[_cfg.currentPosX].boxartSurface;
	SDL_Surface *tmpS=NULL;

	/* Calculate zoomfactor */
	float zoomSurfaceCenterX = 1;
	float zoomSurfaceCenterY = 1;
	if(_CFState != EMULATOR) {
		zoomSurfaceCenterX = _cfg.showBoxarts ? (_cfg.boxartZoomStrX/mainBoxArt->w) : (_cfg.screenZoomStrX/mainBoxArt->w);
		zoomSurfaceCenterY = _cfg.showBoxarts ? (_cfg.boxartZoomStrY/mainBoxArt->h) : (_cfg.screenZoomStrY/mainBoxArt->h);
	}
	tmpS=zoomSurface(mainBoxArt,zoomSurfaceCenterX,zoomSurfaceCenterY,0);  
	mainBoxArt=tmpS;
	

	SDL_Surface	*mainBoxArtUpdated = SDL_DisplayFormatAlpha(mainBoxArt);	

	// draw the shadow 
	if(_CFState != EMULATOR && _cfg.showBoxarts) {
		SDL_Rect imgshadpos;
		imgshadpos.x=((_screen.GetScreenWidth() - defaultBoxArtShadLarge->w)/2)+5;                    // img will be centered on screen
		imgshadpos.y=((_screen.GetScreenHeight() - defaultBoxArtShadLarge->h)/2)+5; 
		imgshadpos.x = (imgshadpos.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
		imgshadpos.y += OffsetY;
		_screen.DrawImage(defaultBoxArtShadLarge, imgshadpos.x, imgshadpos.y, false);
	}

	SDL_Rect imgpos;
	
	
	imgpos.x=(_screen.GetScreenWidth() - mainBoxArt->w)/2;  
	imgpos.y=((_screen.GetScreenHeight()/2) - (mainBoxArt->h/2));
	imgpos.x = (imgpos.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
	imgpos.y += OffsetY;

	_screen.DrawImage(mainBoxArtUpdated, imgpos.x, imgpos.y, true); 	
	_screen.ClearSurface(mainBoxArt);
}

void CoverFlowOnScreen::DrawLeftBoxArt() {

	//Draw Left
	int OffsetSmallY = _cfg.showBoxarts ? _cfg.bortartOffsetSmallY : _cfg.screenOffsetSmallY;
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	
	if(_cfg.currentPosX > 0) {

		int fromLeft = _cfg.currentPosX - _cfg.arraySize;
		if(fromLeft < 0) fromLeft = 0;
		int untilLeft = _cfg.currentPosX;

		for(int i = fromLeft; i < untilLeft; i++) {
			
			SDL_Surface *currentBoxArtLeft = _cfl[i].boxartSurface;

			/* Calculate zoomfactor */
			float zoomSurfaceLeftX = 0.7;
			float zoomSurfaceLeftY = 0.7;
			if(_CFState != EMULATOR) {
				zoomSurfaceLeftX = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallX/currentBoxArtLeft->w) : (_cfg.screenZoomStrSmallX/currentBoxArtLeft->w);
				zoomSurfaceLeftY = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallY/currentBoxArtLeft->h) : (_cfg.screenZoomStrSmallY/currentBoxArtLeft->h);
			}
			
			SDL_Surface *tmpSL=NULL;
			tmpSL=zoomSurface(currentBoxArtLeft,zoomSurfaceLeftX,zoomSurfaceLeftY,0);  // scale the image x0.5 when loaded into memory
			currentBoxArtLeft=tmpSL;

			SDL_Surface	*currentBoxArtLeftUpdated = SDL_DisplayFormatAlpha(currentBoxArtLeft);

			if(_CFState != EMULATOR && _cfg.showBoxarts) {
				SDL_Rect imgshadposLeft;
				imgshadposLeft.x=(((_screen.GetScreenWidth() - defaultBoxArtShadSmall->w)/2)- ((_cfg.currentPosX-i)*distance))+5; 
				imgshadposLeft.y=(((_screen.GetScreenHeight() - defaultBoxArtShadSmall->h)/2))+5; 
				imgshadposLeft.x = (imgshadposLeft.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
				imgshadposLeft.y += OffsetSmallY;
				_screen.DrawImage(defaultBoxArtShadSmall, imgshadposLeft.x, imgshadposLeft.y, false);
			}
		
			SDL_Rect imgposLeft;
			imgposLeft.x=((_screen.GetScreenWidth() - currentBoxArtLeft->w)/2) - ((_cfg.currentPosX-i)*distance); 
			imgposLeft.y=(_screen.GetScreenHeight() - currentBoxArtLeft->h)/2;
			imgposLeft.x = (imgposLeft.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
			imgposLeft.y += OffsetSmallY;
			
			_screen.DrawImage(currentBoxArtLeftUpdated, imgposLeft.x, imgposLeft.y, true);
			_screen.ClearSurface(currentBoxArtLeft);
		} 
	}	
}

void CoverFlowOnScreen::DrawRightBoxArt() {

	//Draw Right
	int OffsetSmallY = _cfg.showBoxarts ? _cfg.bortartOffsetSmallY : _cfg.screenOffsetSmallY;
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	
	if(_cfg.currentPosX < _cfg.gamelistSize) {

		int fromRight = _cfg.currentPosX+1;
		int untilRight = _cfg.currentPosX + _cfg.arraySize;
		if(untilRight > _cfg.gamelistSize) untilRight = _cfg.gamelistSize;

		for(int i = fromRight; i <= untilRight; i++) {		

			SDL_Surface *currentBoxArtRight = _cfl[i].boxartSurface;

			/* Calculate zoomfactor */
			float zoomSurfaceRightX = 0.7;
			float zoomSurfaceRightY = 0.7;
			if(_CFState != EMULATOR) {
				zoomSurfaceRightX = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallX/currentBoxArtRight->w) : (_cfg.screenZoomStrSmallX/currentBoxArtRight->w);
				zoomSurfaceRightY = _cfg.showBoxarts ? (_cfg.boxartZoomStrSmallY/currentBoxArtRight->h) : (_cfg.screenZoomStrSmallY/currentBoxArtRight->h);			
			}
			SDL_Surface *tmpSL=NULL;
			tmpSL=zoomSurface(currentBoxArtRight,zoomSurfaceRightX,zoomSurfaceRightY,0);  // scale the image x0.5 when loaded into memory
			currentBoxArtRight=tmpSL;

			SDL_Surface	*currentBoxArtRightUpdated = SDL_DisplayFormatAlpha(currentBoxArtRight);

			if(_CFState != EMULATOR && _cfg.showBoxarts) {
				SDL_Rect imgshadposRight;
				imgshadposRight.x = (((_screen.GetScreenWidth() - defaultBoxArtShadSmall->w)/2)+ ((i-_cfg.currentPosX)*distance))+5; 
				imgshadposRight.y = (((_screen.GetScreenHeight() - defaultBoxArtShadSmall->h)/2))+5; 
				imgshadposRight.x = (imgshadposRight.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
				imgshadposRight.y += OffsetSmallY;
				_screen.DrawImage(defaultBoxArtShadSmall, imgshadposRight.x, imgshadposRight.y, false);
			}
		
			SDL_Rect imgposRight;
			imgposRight.x=(((_screen.GetScreenWidth() - currentBoxArtRight->w)/2)+ ((i-_cfg.currentPosX)*distance)); 
			imgposRight.y=((_screen.GetScreenHeight() - currentBoxArtRight->h)/2);
			imgposRight.x = (imgposRight.x + _cfa.scrollRangeLeft) - _cfa.scrollRangeRight; //for scrolling
			imgposRight.y += OffsetSmallY;
			
			_screen.DrawImage(currentBoxArtRightUpdated, imgposRight.x, imgposRight.y, true);
			_screen.ClearSurface(currentBoxArtRight);
		} 	
	}
}

void CoverFlowOnScreen::DrawIcons() {

	if(_CFState != EMULATOR) {
		/* BookIcon */
		SDL_Surface	*imgBookIconUpdated = SDL_DisplayFormatAlpha(imgBookIcon);
		SDL_Rect imgposBookIcon;
		imgposBookIcon.x=12; 
		imgposBookIcon.y=8;	

		_screen.DrawImage(imgBookIconUpdated, imgposBookIcon.x, imgposBookIcon.y, true);	
	}

	/* GameIcon */
	SDL_Surface	*imgGameIconUpdated = SDL_DisplayFormatAlpha(imgGameIcon);
	SDL_Rect imgposGameIcon;
	imgposGameIcon.x = _CFState != EMULATOR ? 62 : 12; 
	imgposGameIcon.y=5;	

	_screen.DrawImage(imgGameIconUpdated, imgposGameIcon.x, imgposGameIcon.y, true);
	
	if(_CFState != EMULATOR) {
		/* ConsoleIcon */
		SDL_Surface	*imgConsoleIconUpdated = SDL_DisplayFormatAlpha(imgConsoleIcon);
		SDL_Rect imgposConsoleIcon;
		imgposConsoleIcon.x=127; 
		imgposConsoleIcon.y=4;	

		_screen.DrawImage(imgConsoleIconUpdated, imgposConsoleIcon.x, imgposConsoleIcon.y, true);

		/* Fav icon */
		Sint16 posFavX = _screen.GetScreenWidth() - 20;
	    SDL_Rect favLocation = { posFavX, 7, 0, 0 };
	    if(_cfl[_cfg.currentPosX].isFav) {
			_screen.DrawImage(imgFavIconOn, favLocation.x, favLocation.y, false);
	    }
	    else {
	    	_screen.DrawImage(imgFavIconOff, favLocation.x, favLocation.y, false);
	    }
    }
}


void CoverFlowOnScreen::DrawButtons() {

	Sint16 posBtnA = _screen.GetScreenWidth()-27;
    SDL_Rect rectPosBtnA = { posBtnA, 220, 0, 0 };
    textButtonA = TTF_RenderText_Solid(fontFilename, _CFState == EMULATOR ? "load" : "play", foregroundColor);
    _screen.DrawText(textButtonA, rectPosBtnA.x, rectPosBtnA.y, false);
	rectPosBtnA.x -= 17;
	_screen.DrawImage(imgButtonA, rectPosBtnA.x, rectPosBtnA.y, false);

	if(_CFState != EMULATOR) {
		Sint16 posBtnX = _screen.GetScreenWidth()-84;
	    SDL_Rect rectPosBtnX = { posBtnX, 220, 0, 0 };
	    _screen.DrawText(textButtonX, rectPosBtnX.x, rectPosBtnX.y, false);
		rectPosBtnX.x -= 17;
		_screen.DrawImage(imgButtonX, rectPosBtnX.x, rectPosBtnX.y, false);

		Sint16 posBtnY = _screen.GetScreenWidth()-124;
	    SDL_Rect rectPosBtnY = { posBtnY, 220, 0, 0 };
	    _screen.DrawText(textButtonY, rectPosBtnY.x, rectPosBtnY.y, false); 
		rectPosBtnY.x -= 17;
		_screen.DrawImage(imgButtonY, rectPosBtnY.x, rectPosBtnY.y, false);

		if(_cfg.switchToEmu == 1)  {
			Sint16 posBtnB = _screen.GetScreenWidth()-170;
		    SDL_Rect rectPosBtnB = { posBtnB, 220, 0, 0 };
		    _screen.DrawText(textButtonB, rectPosBtnB.x, rectPosBtnB.y, false); 
			rectPosBtnB.x -= 17;
			_screen.DrawImage(imgButtonB, rectPosBtnB.x, rectPosBtnB.y, false);
		}
		Sint16 posBtnStart = 62;
	    SDL_Rect rectPosBtnStart = { posBtnStart, 220, 0, 0 };
	    _screen.DrawText(textButtonStart, rectPosBtnStart.x, rectPosBtnStart.y, false);
		rectPosBtnStart.x = 46;
		_screen.DrawImage(imgButtonStart, rectPosBtnStart.x, rectPosBtnStart.y, false);		
	}
	
	Sint16 posBtnSelect = 20;
    SDL_Rect rectPosBtnSelect = { posBtnSelect, 220, 0, 0 };
    _screen.DrawText(textButtonSelect, rectPosBtnSelect.x, rectPosBtnSelect.y, false);
	rectPosBtnSelect.x = 3;
	_screen.DrawImage(imgButtonSelect, rectPosBtnSelect.x, rectPosBtnSelect.y, false);
}  

void CoverFlowOnScreen::ScrollImagesLeft() {
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	float scrollSpeed = _cfg.showBoxarts ? _cfg.scrollBoxartSpeed : _cfg.scrollScreenSpeed;

	if(_cfa.isBoxArtMovingLeft && !_cfa.isLoading) {	
		if((_cfa.scrollRangeLeft +(scrollSpeed * deltaTime)) > distance) {
			/* Max target reached, reset */
			_cfa.scrollRangeLeft = distance;
			_cfa.isBoxArtMovingLeft = false;
			_cfa.scrollRangeLeft = 0;	
			_cfg.currentPosX--;
			updateText = true;
		}
		else {
			_cfa.scrollRangeLeft += scrollSpeed * deltaTime;
		}

		RefreshImages();
	}
}

void CoverFlowOnScreen::ScrollImagesRight() {
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	float scrollSpeed = _cfg.showBoxarts ? _cfg.scrollBoxartSpeed : _cfg.scrollScreenSpeed;

	if(_cfa.isBoxArtMovingRight && !_cfa.isLoading) {
		if((_cfa.scrollRangeRight + (scrollSpeed * deltaTime))  > distance) {
			_cfa.scrollRangeRight = distance;
			_cfa.isBoxArtMovingRight = false;
			_cfa.scrollRangeRight = 0;
			_cfg.currentPosX++;	
			updateText = true;	
		}
		else {
			_cfa.scrollRangeRight += scrollSpeed * deltaTime;
		}

		RefreshImages();
	}
}

void CoverFlowOnScreen::ScrollSkipImagesLeft() {
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	if(_cfa.isBoxArtSkipLeft && !_cfa.isLoading && !_cfa.isBoxArtSkipRight) {	
		if((_cfa.scrollRangeLeft +(_cfg.scrollSpeedFast * deltaTime)) > distance) {
			/* Max target reached, reset */
			_cfa.scrollRangeLeft = distance;
			_cfa.currentSkipImage--;
			_cfa.scrollRangeLeft = 0;	
			_cfg.currentPosX--;
			updateText = true;
			LoadImages(_cfg.currentPosX-3); 
		}
		else {
			_cfa.scrollRangeLeft += _cfg.scrollSpeedFast * deltaTime;
		}

		if(_cfa.currentSkipImage < 0 || _cfg.currentPosX == 0) {
			_cfa.isBoxArtSkipLeft = false;
		}

		RefreshImages();
	}
}

void CoverFlowOnScreen::ScrollSkipImagesRight() {
	int distance = _cfg.showBoxarts ? _cfg.boxartDistance : _cfg.screenDistance;
	if(_cfa.isBoxArtSkipRight && !_cfa.isLoading && !_cfa.isBoxArtSkipLeft) {	
		if((_cfa.scrollRangeRight + (_cfg.scrollSpeedFast * deltaTime))  > distance) {
			_cfa.scrollRangeRight = distance;
			_cfa.currentSkipImage--;
			_cfa.scrollRangeRight = 0;
			_cfg.currentPosX++;	
			updateText = true;	
			LoadImages(_cfg.currentPosX+3);  
		}
		else {
			_cfa.scrollRangeRight += _cfg.scrollSpeedFast * deltaTime;
		}

		if(_cfa.currentSkipImage < 0 || _cfg.currentPosX == _cfg.gamelistSize) {
			_cfa.isBoxArtSkipRight = false;
		}		

		RefreshImages();
	}
}

void CoverFlowOnScreen::DrawThemeScreen() {
	
	if(_themeList.size() > 0) {
		
		/* Textbox */
		SDL_Surface* msgboxSurfacePre = IMG_Load(("assets/images/msgbox.png"));
		imgMsgboxSurface = SDL_DisplayFormatAlpha(msgboxSurfacePre);
		Sint16 posImgX = (_screen.GetScreenWidth() / 2 )  - (imgMsgboxSurface->w /2);
		Sint16 posImgy = (_screen.GetScreenHeight() / 2 )- (imgMsgboxSurface->h /2);

		_screen.DrawImage(imgMsgboxSurface, posImgX, posImgy, true);


		textThemeMsg = TTF_RenderText_Solid(fontFilename, "Select a theme:", foregroundColor);
		string currentThemeText = _themeList[_themePosition];

		if(_themePosition > 0) currentThemeText = "< " + currentThemeText;
		if((_themePosition+1) < _themeList.size()) currentThemeText += " >";
		
		textCurrentThemeMsg = TTF_RenderText_Solid(fontFilename, currentThemeText.c_str(), foregroundColor);

		Sint16 posTextX = (_screen.GetScreenWidth() / 2 )  - (textThemeMsg->w /2);
		Sint16 posTexty = (_screen.GetScreenHeight() / 2 );
		Sint16 posTextNameX = (_screen.GetScreenWidth() / 2 )  - (textCurrentThemeMsg->w /2);
		Sint16 posTextNamey = (_screen.GetScreenHeight() / 2 );

		_screen.DrawText(textThemeMsg, posTextX, posTexty-20, true);	
		_screen.DrawText(textCurrentThemeMsg, posTextNameX, posTextNamey, true);

		Sint16 posBtnA = _screen.GetScreenWidth()-50;
	    SDL_Rect rectPosBtnA = { posBtnA, 220, 0, 0 };
	    textButtonA = TTF_RenderText_Solid(fontFilename, "Confirm", foregroundColor);
	    _screen.DrawText(textButtonA, rectPosBtnA.x, rectPosBtnA.y, false);
		rectPosBtnA.x -= 17;
		_screen.DrawImage(imgButtonA, rectPosBtnA.x, rectPosBtnA.y, false);		
	}
}

void CoverFlowOnScreen::Cleanup() {
	
	/* free the background surface */
	SDL_FreeSurface(imgBackground);
	/* Clear buttons */
	SDL_FreeSurface(textButtonY); 
	SDL_FreeSurface(textButtonX); 
	if(_cfg.switchToEmu == 1) SDL_FreeSurface(textButtonB); 
	SDL_FreeSurface(textButtonA); 
	SDL_FreeSurface(imgButtonY); 
	SDL_FreeSurface(imgButtonX); 
	if(_cfg.switchToEmu == 1) SDL_FreeSurface(imgButtonB); 
	SDL_FreeSurface(imgButtonA); 

	/* clear text */
	TTF_CloseFont(fontCharacter);
    TTF_CloseFont(fontGameList);
    TTF_CloseFont(fontConsolename);
    TTF_CloseFont(fontFilename);

    fontCharacter = NULL;
    fontGameList = NULL;
    fontConsolename = NULL;
    fontFilename = NULL;

}

/*************/
/* TEXT PART */
/*************/

void CoverFlowOnScreen::DrawTextError(string errorMsg) {	
	textErrorMsg = TTF_RenderText_Solid(fontFilename, errorMsg.c_str(), foregroundColor);

	Sint16 posTextX = (_screen.GetScreenWidth() / 2 )  - (textErrorMsg->w /2);
	Sint16 posTexty = (_screen.GetScreenHeight() / 2 );
	SDL_Rect textLocation = { posTextX, posTexty, 0, 0 };
	_screen.DrawText(textErrorMsg, textLocation.x, textLocation.y, true);	
}

void CoverFlowOnScreen::ReloadCharacter() {
	if(_CFState != EMULATOR) {
    	characterSurface = TTF_RenderText_Solid(fontCharacter, GetCurrentCharacter().c_str(), foregroundColor);
    	SDL_Rect characterLocation = { 25, 6, 0, 0 };
    	_screen.DrawText(characterSurface, characterLocation.x, characterLocation.y, true);
	}
}

void CoverFlowOnScreen::ReloadGamePosition() {    
    characterSurface = TTF_RenderText_Solid(fontGameList, GetGameposition().c_str(), foregroundColor);
    Sint16 posX = _CFState != EMULATOR ? 81 : 35;
    SDL_Rect characterLocation = { posX, 6, 0, 0 };
    _screen.DrawText(characterSurface, characterLocation.x, characterLocation.y, true);
}

void CoverFlowOnScreen::ReloadConsoleName() {    
	if(_CFState != EMULATOR) {
	    characterSurface = TTF_RenderText_Solid(fontConsolename, _cfg.emulatorType.c_str(), foregroundColor);
	    SDL_Rect characterLocation = { 148, 6, 0, 0 };
	    _screen.DrawText(characterSurface, characterLocation.x, characterLocation.y, true);
	}
}

void CoverFlowOnScreen::ReloadText() {
    textSurface = TTF_RenderText_Solid(fontFilename, GetCurrentFilename().c_str(), foregroundColor);   
    Sint16 posTextX = (_screen.GetScreenWidth() / 2 )  - (textSurface->w /2); 
    Sint16 posTexty = _cfg.showBoxarts ? _cfg.bortartFilenamePosY : _cfg.screenFilenamePosY;
    SDL_Rect textLocation = { posTextX, posTexty, 0, 0 };
    _screen.DrawText(textSurface, textLocation.x, textLocation.y, true);    
}    

void CoverFlowOnScreen::SwitchView() {	
	_cfa.isLoading = true;
	_cfg.showBoxarts = !_cfg.showBoxarts;
	LoadAssets();
	PreLoadImages();
	RefreshImages();
	updateCharacter = true;
	updateText = true;
	_cfa.isLoading = false;
} 

void CoverFlowOnScreen::findfile(string filename, string romPath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(romPath.c_str());


    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
			
			string currentFile = dp->d_name;
			
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, "media") != 0)
        {
            if(dp->d_type != DT_DIR){

				
            	if(currentFile == filename)
            	{
            		string currentpath = romPath;
            		_fullFilePath = currentpath  + "/" + currentFile;
				}
			}
			else {

	            // Construct new path from our base path
	            strcpy(path, romPath.c_str());
	            strcat(path, "/");
	            strcat(path, dp->d_name);
	            findfile(filename, path);
            }
        }
    }

    closedir(dir);
}

void CoverFlowOnScreen::RunGame() {	
	SDL_Quit();
	TTF_Quit();

	int i = 123;
	string posXStr;
	string posYStr;
	stringstream ssPosX;
	stringstream ssPosY;
	ssPosX << _cfg.currentPosX;
	posXStr = ssPosX.str();
	ssPosY << _cfg.currentPosY;
	posYStr = ssPosY.str();

	findfile(GetCurrentFilename(), _cfg.romPath);


	_helper.executeCommand (_cfg.emulatorPath, _cfg.emulatorExec, _cfg.emulatorParams, _fullFilePath, _cfg.emulatorType, posXStr, posYStr);
  	//Init();		
  	//InitEmulator();	
}



/***************/
/* GLOBQL PART */
/***************/

const string CoverFlowOnScreen::GetCurrentFilename() {
	string filename = _cfl[_cfg.currentPosX].fileName;
	return filename;
}

const string CoverFlowOnScreen::GetCurrentCharacter() {
	string character = _characterList[_cfg.currentPosY];
	return character;
}

const string CoverFlowOnScreen::GetGameposition() {
	std::stringstream sstm;
	sstm << (_cfg.currentPosX+1) << "/" << (_cfg.gamelistSize+1);
	string gamePos = sstm.str();
	return gamePos;
}

/*****************/
/* Controll PART */
/*****************/

void CoverFlowOnScreen::CheckController() {

	if(btnPressedUp) {
		if(_CFState != EMULATOR && !_showThemeScreen) {
			PrevCharacter();
		}			
		btnPressedUp = false;
	}
	if(btnPressedDown) {
		if(_CFState != EMULATOR && !_showThemeScreen) {
			NextCharacter();
		}		
		btnPressedDown = false;
	}
	if(btnPressedLeft) {
		if(!_cfa.isBoxArtSkipRight && !_cfa.isBoxArtSkipLeft && !_cfa.isBoxArtMovingRight && !_cfa.isBoxArtMovingLeft && !_showThemeScreen) {
			if(_cfg.currentPosX > 0 && !_cfa.isBoxArtSkipLeft) {
				LoadImages(_cfg.currentPosX-4);
				_cfa.isBoxArtMovingLeft = true;
			}
					
		}
		if(_showThemeScreen) {
			if(_themePosition > 0) {			
				_themePosition--;
			}			
		}			
		btnPressedLeft = false;
	}
	if(btnPressedRight) {
		if(!_cfa.isBoxArtSkipRight && !_cfa.isBoxArtSkipLeft && !_cfa.isBoxArtMovingRight && !_cfa.isBoxArtMovingLeft && !_showThemeScreen) {			
			if(_cfg.currentPosX < _cfg.gamelistSize && !_cfa.isBoxArtSkipRight) {
				LoadImages(_cfg.currentPosX+4);
				_cfa.isBoxArtMovingRight = true;
			}					
		}
		if(_showThemeScreen) {
			if((_themePosition+1) < (_themeList.size())) {			
				_themePosition++;
			}			
		}		
		btnPressedRight = false;
	}
	if(btnPressedY) {
		if(_CFState != EMULATOR && !_showThemeScreen) {
			UpdateFavorite();
		}
		btnPressedY = false;
	}
	if(btnPressedX) {
		if(_CFState != EMULATOR && !_showThemeScreen) {
			SwitchView();		
		}
		btnPressedX = false;
	}
	if(btnPressedB) {
		if(_CFState != EMULATOR && _cfg.switchToEmu == 1 && !_showThemeScreen) {
			_CFState = EMULATOR;
			switchEnvironment = true;
		}
		btnPressedB = false;
	}
	if(btnPressedA) {
		if(_CFState != EMULATOR && !_showThemeScreen) {
			RunGame();
			_quit = true;
		}
		if(_CFState == EMULATOR && !_showThemeScreen) {
			_cfg.emulatorType = _cfl[_cfg.currentPosX].fileName;
			_CFState = GAMELIST;
			switchEnvironment = true;
		}
		if(_showThemeScreen) {
			vector<string> _themeSave;
			_themeSave.push_back("#current theme");
			_themeSave.push_back("theme=" + _themeList[_themePosition]);
			_helper.WriteVectorToFile("themes/" + _cfg.emulatorType + "/emulator.cfg", _themeSave);
			_themePosition=0;
			switchEnvironment = true;
		}
		btnPressedA = false;
	}
	if(btnPressedL) {
		if(!_cfa.isBoxArtSkipRight && !_cfa.isBoxArtSkipLeft && !_cfa.isBoxArtMovingRight && !_cfa.isBoxArtMovingLeft && _cfg.currentPosX < _cfg.gamelistSize && !_showThemeScreen) {
				_cfa.currentSkipImage = _cfg.skipImages;
				_cfa.isBoxArtSkipRight = true;
		}		
		btnPressedL = false;
	}
	if(btnPressedR) {
		if(!_cfa.isBoxArtSkipRight && !_cfa.isBoxArtSkipLeft && !_cfa.isBoxArtMovingRight && !_cfa.isBoxArtMovingLeft && _cfg.currentPosX > 0 && !_showThemeScreen) {
			_cfa.currentSkipImage = _cfg.skipImages;
				_cfa.isBoxArtSkipLeft = true;
		}	          	
		btnPressedR = false;
	}
	if(btnPressedSelect) {
		_quit = true;
		btnPressedSelect = false;
	}
	if(btnPressedStart) {	
		if(!_showThemeScreen && _CFState != EMULATOR) {
			_showThemeScreen = true;
		}
		btnPressedStart = false;
	}											
 
}

void CoverFlowOnScreen::GetController(SDL_Event event) {
/* look for an event */
	if (SDL_PollEvent(&event)) {
	  /* an event was found */
	  switch (event.type) {  	
	    /* close button clicked */
	    case SDL_QUIT:
			_quit = true;
			break;
	    /* handle the keyboard */
	    case SDL_KEYDOWN:
	      switch (event.key.keysym.sym) {	      	
	        case SDLK_ESCAPE:
	        case SDLK_q:
	        	btnPressedSelect = true;
				break;
	        case SDLK_UP:
	        	btnPressedUp = true;
	          break;
	        case SDLK_DOWN:
	        	btnPressedDown = true;
	          break;	 
	        case SDLK_LCTRL:
	        	btnPressedB = true;	   
	        	break;         
			case SDLK_BACKSPACE:
				btnPressedL = true;					
	          break;	
			case SDLK_TAB:
				btnPressedR = true;				     
				break;
			case SDLK_SPACE:
				btnPressedY = true;
				break;
			case SDLK_LSHIFT:
				btnPressedX = true;
				break;				
		    case SDLK_LALT:
		    	btnPressedA = true;
		      break;	
		    case SDLK_RETURN:
		    	btnPressedStart = true;	   
		    	break;
		    case SDLK_RIGHT:
		    	btnPressedRight = true;
		    	break;       
		    case SDLK_LEFT:
		    	btnPressedLeft = true;
		    	break;       
	      }
	      break;
	  }
	}
	
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_RIGHT] && !_showThemeScreen) {
		btnPressedRight = true;
	}	
	else if(keystate[SDLK_LEFT] && !_showThemeScreen){
		btnPressedLeft = true;
	}	

	CheckController();
	
}