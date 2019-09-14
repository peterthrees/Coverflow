#include <iostream>
#include <fstream>
#include "string.h"
#include <cstdio>
#include <ctime>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;


int main(int argc, char *argv[]) {

	cout << "start invoker " << endl;
	char menuDirectory[100] = "";
	string directory=argv[1];
	string executable=argv[2];
	string parameters=argv[3];
	string fileToBeExecutedWithFullPath=argv[4];
	string emulatorType=argv[5];
	string posX=argv[6];
	string posY=argv[7];
	getcwd(menuDirectory, sizeof(menuDirectory));

	int ret = chdir(directory.c_str());

	if(ret == 0) {
		string directAndExec = directory + executable;	

		if(!fork())
		{
			if(parameters == "") {
				execlp(directAndExec.c_str(), executable.c_str(), fileToBeExecutedWithFullPath.c_str(), NULL);
			}
			else {
				execlp(directAndExec.c_str(), executable.c_str(), parameters.c_str(), fileToBeExecutedWithFullPath.c_str(), NULL);
			}
		}
		wait(NULL);	  
	}  
	
	chdir(menuDirectory);
	execlp("./coverflow","coverflow", emulatorType.c_str(), posX.c_str(), posY.c_str(),  NULL);
}
