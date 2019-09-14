
//Configuration.cpp
#include <iostream>
#include <fstream>
#include "Helper.h"
#include "Configuration.h"
#include "string.h"
#include <cstdio>
#include <ctime>
#include <vector>
#include <iterator>
#include <unistd.h>

using namespace std;

    

void Helper::Init()
{   
	start = clock();
}

void Helper::GetFileContent(string fileName, vector<string> &vecOfStrs)
{
 
	// Open the File
	ifstream in(fileName.c_str());
 
	// Check if object is valid
	if(!in)
	{
		//cerr << "Cannot open the File : "<<fileName<<endl;
	}
	else { 
		string str;
		// Read the next line from File untill it reaches the end.
		while (std::getline(in, str))
		{
			// Line contains string of length > 0 then save it in vector
			if(str.size() > 0)
				vecOfStrs.push_back(str);
		}
		//Close The File
		in.close();
	}
}

void Helper::WriteVectorToFile(string fileName, const vector<string> &message)
{   

	std::ofstream output_file(fileName);
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(message.begin(), message.end(), output_iterator);
  
}



void Helper::WriteTextToLogFile(const string &message)
{   

  if(_cfg.logging == 1) {
	  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	  ofstream myfile;
	  myfile.open ("logging.txt", ios_base::app);
	  myfile << message << " ?? Duraction " << duration << ";" << "\n";
	  myfile.close(); 
  }
  
}


void Helper::executeCommand (string emulatorPath, string emulatorName, string emulatorParams, string fileToBeExecuted, string emulatorType, string posX, string posY) {

	cout << "start inv cov:" << posX << " " << posY << endl;
	execlp("./invoker.elf","invoker.elf", emulatorPath.c_str(), emulatorName.c_str(), emulatorParams.c_str(), fileToBeExecuted.c_str(), emulatorType.c_str(), posX.c_str(), posY.c_str(), NULL);
	cout << "done inv cov" << endl;
}