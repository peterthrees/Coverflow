// filehandler.h
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "string.h"
#include <vector>
#include "Configuration.h"
#include "Helper.h"





using namespace std;

class FileHandler {
public:
	vector<string> fileNames;
	vector<string> boxArtNames;
	vector<string> screenshotNames;	
	vector<string> _favList;

	/* Config */
	Configuration::CFConfig _cfg;

	Helper _helper;

	struct FileContainer
	{
		string character = "";
		string fileName = "";
		string boxartName = "";
		string screenshotName = "";
		bool useDefault = false;
		bool useDefaultScreen = false;
	};

	/* Console */
	struct ConsoleContainer
	{
		SDL_Rect position;
		SDL_Surface* consoleImage;
		string consoleName;
	};
	int CONSOLE_WIDTH = 105;
	int CONSOLE_HEIGHT = 105;
	vector<ConsoleContainer> _consoleContainer;
	vector<FileContainer> _fileCollection;
	vector<string> _characterList;

	FileHandler();
	void listFilesRecursively(string basePath);
	void LoadEmulators();
	void LoadFavList();
	void LoadFiles(); 
	void SetCharacterList();	
	void SetCharacterEmulatorList();
	
	const string getext(string filename);
	const vector<string> split(string str, string token);
	const vector<string> GetThemes();

};

#endif