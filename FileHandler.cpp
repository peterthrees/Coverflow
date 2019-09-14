 
//DrawOnScreen.cpp
#include <iostream>
#include <vector>
#include <stdbool.h>
#include <stdio.h> 
#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <algorithm>
#include "FileHandler.h"
#include "Helper.h"

using namespace std;



FileHandler::FileHandler() { 
	
}

void FileHandler::LoadEmulators() {

    string themePath = "themes/";

    /*** File Reader */    
    struct dirent *de;  // Pointer for directory entry 
    DIR *dr = opendir(themePath.c_str()); 

    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" );        
    } 

    while ((de = readdir(dr)) != NULL) {
    	string currentEmulator = de->d_name;
    	if(currentEmulator != "." && currentEmulator != "..") {
			FileContainer _fileContainer;
			_fileContainer.fileName = de->d_name;
			_fileContainer.boxartName = themePath + de->d_name + "/console.png";
			_fileContainer.useDefault = false;
			_fileContainer.character = "$";
			_fileCollection.push_back(_fileContainer);
    	}
    }
    closedir(dr);   
}

void FileHandler::LoadFavList() {
	_helper.GetFileContent(_cfg.favSave, _favList);
}

void FileHandler::listFilesRecursively(string basePath)
{
    char path[1000];
    struct dirent *dp;

    /* Get extensionlist */
	vector<string> emulatorExt = split(_cfg.emulatorExtensions, ";");

	/* Create opendir stream */
    DIR *dir = opendir(basePath.c_str());

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, "media") != 0)
        {
            //printf("%s\n", dp->d_name);
            string currentFile = dp->d_name;
            if(dp->d_type != DT_DIR){

				string currentExtension =getext(currentFile);
				if (std::find(emulatorExt.begin(), emulatorExt.end(), currentExtension) != emulatorExt.end() || _cfg.emulatorExtensions == "*")
				{
					fileNames.push_back(currentFile);
				}
			}
			else {

	            // Construct new path from our base path
	            strcpy(path, basePath.c_str());
	            strcat(path, "/");
	            strcat(path, dp->d_name);

	            listFilesRecursively(path);
            }
        }
    }

    closedir(dir);
}


void FileHandler::LoadFiles() {

	_helper.WriteTextToLogFile("FileHandler: LoadFiles start"); //log

	/*** File Reader */  
	_helper.WriteTextToLogFile("FileHandler: LoadFiles roms"); //log  

	// Load roms
	listFilesRecursively(_cfg.romPath);
	  
	// Init dir
	struct dirent *de;  // Pointer for directory entry 
	DIR *dr;
	/*** Boxart Reader */ 	
	
	dr = opendir(_cfg.boxartPath.c_str()); 

	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{ 
		printf("Could not open boxarts directory" ); 	
		_helper.WriteTextToLogFile("Could not open boxarts directory"); //log
	} 
	else {
		// for readdir() 
		while ((de = readdir(dr)) != NULL) {
			string currentFile = de->d_name;

			if(currentFile.length() > 4 && currentFile.find(".png")  != string::npos) {
				boxArtNames.push_back(currentFile); 			
			}
		}
		closedir(dr); 	
	}

	for(int i = 0; i < boxArtNames.size(); i++) {
		_helper.WriteTextToLogFile(boxArtNames[i]); //log
	}

	/*** Screenshot Reader */ 	
	_helper.WriteTextToLogFile("FileHandler: LoadFiles screenshots"); //log 
	dr = opendir(_cfg.screenshotsPath.c_str()); 

	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{ 
		printf("Could not open current directory" ); 		
	} 
	else {
		// for readdir() 
		while ((de = readdir(dr)) != NULL) {
			string currentFile = de->d_name;
			if(currentFile.length() > 4 && currentFile.find(".png")  != string::npos) {
				screenshotNames.push_back(currentFile); 			
			}
		}
		closedir(dr); 	
	}

	/****** END FILE READER *****/
	_helper.WriteTextToLogFile("FileHandler: LoadFiles sort"); //log  
	std::sort(fileNames.begin(), fileNames.end());

	_helper.WriteTextToLogFile("FileHandler: Collect"); //log  	
	for(int i = 0; i < fileNames.size(); i++) {

		FileContainer _fileContainer;
		_fileContainer.fileName = fileNames[i];
		//set character
		char currentCharacter = fileNames[i][0];
		if(!isalnum(fileNames[i][0])) _fileContainer.character = "#";
		else if (isdigit(fileNames[i][0])) _fileContainer.character = "0";
		else _fileContainer.character = toupper(fileNames[i][0]);

		string imageFile = fileNames[i];
		size_t lastindex = imageFile.find_last_of("."); 
		
		/* boxart */
		_fileContainer.useDefault = false;
		string boxartFile = imageFile.substr(0, lastindex) + ".png";

		if (std::find(boxArtNames.begin(), boxArtNames.end(), boxartFile) != boxArtNames.end())
		{
			_fileContainer.boxartName = _cfg.boxartPath + boxartFile;
			_helper.WriteTextToLogFile(_fileContainer.boxartName); //log
		}
		else {
			_fileContainer.boxartName = _cfg.imgDefaultBoxart;
			_fileContainer.useDefault = true;
		}

		/* screenshot */
		string screenFile = imageFile.substr(0, lastindex) + ".png";
		
		if (std::find(screenshotNames.begin(), screenshotNames.end(), screenFile) != screenshotNames.end())
		{
			_fileContainer.screenshotName = _cfg.screenshotsPath + screenFile;
		}
		else {
			_fileContainer.screenshotName = "ignore";
			_fileContainer.useDefaultScreen = true;
		}
		
		/* Add container */
		_fileCollection.push_back(_fileContainer);
				
	} 
}

void FileHandler::SetCharacterEmulatorList() {
	_characterList.push_back("$");
}

void FileHandler::SetCharacterList() {

	/* Favourite */
	_characterList.push_back("*");

	/* None alphanumeric */
	for(int i = 0; i < fileNames.size(); i++) {
		if(!isalnum(fileNames[i][0])) {	_characterList.push_back("#"); break; }
	}

	/* Numeric */
	for(int i = 0; i < fileNames.size(); i++) {
		if(isdigit(fileNames[i][0])) {_characterList.push_back("0"); break; }
	}	

	/* A */
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'A') {_characterList.push_back("A");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'B') {_characterList.push_back("B");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'C') {_characterList.push_back("C");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'D') {_characterList.push_back("D");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'E') {_characterList.push_back("E");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'F') {_characterList.push_back("F");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'G') {_characterList.push_back("G");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'H') {_characterList.push_back("H");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'I') {_characterList.push_back("I");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'J') {_characterList.push_back("J");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'K') {_characterList.push_back("K");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'L') {_characterList.push_back("L");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'M') {_characterList.push_back("M");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'N') {_characterList.push_back("N");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'O') {_characterList.push_back("O");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'P') {_characterList.push_back("P");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'Q') {_characterList.push_back("Q");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'R') {_characterList.push_back("R");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'S') {_characterList.push_back("S");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'T') {_characterList.push_back("T");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'U') {_characterList.push_back("U");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'V') {_characterList.push_back("V");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'W') {_characterList.push_back("W");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'X') {_characterList.push_back("X");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'Y') {_characterList.push_back("Y");	break;}}	
	for(int i = 0; i < fileNames.size(); i++) {	if(toupper(fileNames[i][0]) == 'Z') {_characterList.push_back("Z");	break;}}	

}




const string FileHandler::getext(string filename)
{
	int pos;
	string ext;
	pos = filename.find_last_of('.');
	if(pos == -1) return "";

	ext = filename.substr(pos, -1);
	transform(ext.begin(), ext.end(),ext.begin(), ::toupper);
	return ext;	
}

const vector<string> FileHandler::split(string str, string token){
	transform(str.begin(), str.end(),str.begin(), ::toupper);
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

const vector<string> FileHandler::GetThemes() {
	vector<string>result;


    string themePath = "themes/" + _cfg.emulatorType;

    /*** File Reader */    
    struct dirent *de;  // Pointer for directory entry 
    DIR *dr = opendir(themePath.c_str()); 

    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" );        
    } 

    while ((de = readdir(dr)) != NULL) {
    	    string currentFile = de->d_name;
            if(de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
				result.push_back(currentFile);
			}
    	
    }
    closedir(dr);  

	return result;
}