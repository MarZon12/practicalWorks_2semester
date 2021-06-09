#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

#include "BinaryTree.h"

class CourseWork
{
public:
	CourseWork();

	//       -MENU-       //

	void menu_main();

	void menu_WorkWithTree();

private:
	struct extractIntFromStringResult {
		int intValue;
		bool success;
		int nextCharAfterIntPos;
		extractIntFromStringResult() :intValue(0), success(false), nextCharAfterIntPos(-1) {};
		extractIntFromStringResult(int intValue_, bool success_, int nextCharAfterIntPos_) :intValue(intValue_), success(success_), nextCharAfterIntPos(nextCharAfterIntPos_) {};
	};
	
	BinaryTree binTree;

	bool useNullWhenPrint;

	std::string fileName;


	//       -NOTIFICATIONS-       //

	std::vector<std::string> notificationsToDisplay;

	void addNotification(std::string notification);

	void displayNotifications();


	//       -OTHER STUFF-       //

	bool bTimingIsEnabled;

	void displayTimerResultWithCheck(const std::chrono::steady_clock::time_point& timerStart, const std::chrono::steady_clock::time_point& timerStop);

	extractIntFromStringResult extractIntFromString(std::string stringToAnalyze, int pos);

	void displayClsWithPause();
};

