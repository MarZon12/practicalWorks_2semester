#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

#include "improvedArray.h"

class practicalWork2
{
public:
	practicalWork2();
	~practicalWork2() {};


	void menu_main();

	void menu_IA();
	void menu_IA_WorkWithArray();

	void menu_DLList();

private:
	struct extractIntFromStringResult {
		int intValue;
		bool success;
		int nextCharAfterIntPos;
		extractIntFromStringResult() :intValue(0), success(false), nextCharAfterIntPos(-1) {};
		extractIntFromStringResult(int intValue_, bool success_, int nextCharAfterIntPos_) :intValue(intValue_), success(success_), nextCharAfterIntPos(nextCharAfterIntPos_) {};
	};
	
	MZ_ia::improvedArray<int> arrIA;

	std::string fileName;

	bool bTimingIsEnabled;

	void displayTimerResultWithCheck(const std::chrono::steady_clock::time_point& timerStart, const std::chrono::steady_clock::time_point& timerStop);

	extractIntFromStringResult extractIntFromString(std::string stringToAnalyze, int pos);

	void displayClsWithPause();

	std::vector<std::string> notificationsToDisplay;

	//Notifications
	void addNotification(std::string notification);
	void displayNotifications();

	void displayArray();
};

