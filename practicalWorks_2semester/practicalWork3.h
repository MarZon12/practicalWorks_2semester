#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "PolishNotation.h"

class practicalWork3
{
public:
	//       -MENU-       //

	void menu_main();

private:
	PolishNotation notation;

	//       -NOTIFICATIONS-       //

	std::vector<std::string> notificationsToDisplay;

	void addNotification(std::string notification);

	void displayNotifications();
};
