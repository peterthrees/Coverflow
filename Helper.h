// helper.h
#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <vector>
#include "Configuration.h"

using namespace std;

class Helper {
public:

	clock_t start;
    double duration;
    Configuration::CFConfig _cfg;

    void Init();

    void GetFileContent(string fileName, vector<string> &vecOfStrs);

    void WriteVectorToFile(string fileName, const vector<string> &message);

	void WriteTextToLogFile(const string &message);

	void executeCommand (string emulatorPath, string emulatorName, string emulatorParams, string fileToBeExecuted, string emulatorType, string posX, string posY);
};

#endif