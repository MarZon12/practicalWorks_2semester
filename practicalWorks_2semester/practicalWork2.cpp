#include "practicalWork2.h"

practicalWork2::practicalWork2() {
	fileName = "file.txt";
	bTimingIsEnabled = true;
};

//Notifications
void practicalWork2::addNotification(std::string notification) {
	notificationsToDisplay.push_back(notification);
};
void practicalWork2::displayNotifications() {
	if (!notificationsToDisplay.empty())
	{
		for (int i = 0; i < notificationsToDisplay.size(); i++)
			std::cout << "Notification: " << notificationsToDisplay[i] << std::endl;
		notificationsToDisplay.clear();
	}
}

//pos - first character after int found in string
practicalWork2::extractIntFromStringResult practicalWork2::extractIntFromString(std::string stringToAnalyze, int pos) {
	if (pos >= stringToAnalyze.length() || pos < 0)
		return practicalWork2::extractIntFromStringResult();
	std::string tempString;
	std::string tempSubString = stringToAnalyze.substr(pos);
	int tempValue;
	int tempPos;
	std::istringstream iss(tempSubString);
	std::ostringstream oss;
	iss >> tempValue;
	oss << tempValue;
	tempString = oss.str();
	tempPos = tempSubString.find(tempString);
	if (tempPos != std::string::npos)
		return practicalWork2::extractIntFromStringResult(tempValue, true, tempPos + tempString.length() + pos);
	else
		return practicalWork2::extractIntFromStringResult();
};

void practicalWork2::displayTimerResultWithCheck(const std::chrono::steady_clock::time_point& timerStart, const std::chrono::steady_clock::time_point& timerStop) {
	if (bTimingIsEnabled)
		std::cout << "Time taken to complete: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timerStop - timerStart).count() << " ns" << std::endl;
}

void practicalWork2::displayClsWithPause() {
	system("pause");
	system("cls");
}



void practicalWork2::menu_main() {
	
	int choise;
	while (true)
	{
		displayNotifications();
		
		std::cout << "Practical work 2\n"
			<< "1 - Integer array\n"
			<< "2 - Doubly linked list\n"
			<< "3 - Exit\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:
			menu_IA();
			break;
		case 2:
			menu_DLList();
			break;
		default:
			return;
			break;
		}
	}
}



void practicalWork2::displayArray() {
	std::cout << "Array: ";
	for (size_t i = 0; i < arrIA.size(); i++)
		std::cout << arrIA[i] << ' ';
	std::cout << std::endl;
}

void practicalWork2::menu_IA() {

	int choise,
		pos = 0;

	std::string inputString;

	practicalWork2::extractIntFromStringResult extractionResult;

	std::ifstream fin;

	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerStop;

	srand(time(NULL));

	while (true)
	{
		displayNotifications();
		
		std::cout << "Practical work 2: \n"
			<< "1 - Create an array according to the entered size. Then fill in with random numbers.\n"
			<< "2 - Create an array based on the entered elements. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (enter on one line))\n"
			<< "3 - Create an array of read elements from a file. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (must be written in one line))\n"
			<< "4 - Toggle timing. (current state = " << (bTimingIsEnabled ? "enabled" : "disabled") << ")\n"
			<< "5 - Work with an array\n"
			<< "6 - Back\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:
			std::cout << "Enter array size: ";
			std::cin >> choise;

			timerStart = std::chrono::steady_clock::now();
			arrIA.resize(choise);
			for (size_t i = 0; i < arrIA.size(); i++)
				arrIA[i] = rand() % 123;
			timerStop = std::chrono::steady_clock::now();

			displayArray();

			displayTimerResultWithCheck(timerStart, timerStop);

			displayClsWithPause();
			break;
		case 2:
			std::cout << "Enter the elements of the array (in one line): ";

			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			std::getline(std::cin, inputString);

			timerStart = std::chrono::steady_clock::now();
			pos = 0;
			do
			{
				extractionResult = extractIntFromString(inputString, pos);
				if (extractionResult.success)
				{
					arrIA.push_back(extractionResult.intValue);
					pos = extractionResult.nextCharAfterIntPos;
				}
			} while (extractionResult.success);
			timerStop = std::chrono::steady_clock::now();

			displayArray();

			displayTimerResultWithCheck(timerStart, timerStop);

			displayClsWithPause();
			break;
		case 3:
			fin.open(fileName);

			if (!fin.is_open())
			{
				std::cout << "Could not open file \"file.txt\"." << std::endl;
				displayClsWithPause();
				break;
			}

			std::getline(fin, inputString);

			pos = 0;
			timerStart = std::chrono::steady_clock::now();
			do
			{
				extractionResult = extractIntFromString(inputString, pos);
				if (extractionResult.success)
				{
					arrIA.push_back(extractionResult.intValue);
					pos = extractionResult.nextCharAfterIntPos;
				}
			} while (extractionResult.success);
			timerStop = std::chrono::steady_clock::now();

			displayArray();

			displayClsWithPause();
			break;
		case 4:
			bTimingIsEnabled = !bTimingIsEnabled;
			addNotification("Timing state has been updated");
			break;
		case 5:
			menu_IA_WorkWithArray();
			break;
		default:
			return;
			break;
		}
	}
}

void practicalWork2::menu_IA_WorkWithArray() {

	int choise,
		index,
		value;

	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerStop;

	srand(time(NULL));

	while (true)
	{
		displayNotifications();
		
		std::cout << "Practical work 2: \n"
			<< "1 - Insert item into array (by index)\n"
			<< "2 - Remove item in array (by index)\n"
			<< "3 - Remove item in array (by value)\n"
			<< "4 - Get the value of an array element (by index)\n"
			<< "5 - Get the index of an element of an array (by value)\n"
			<< "6 - Back\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:
			displayArray();

			std::cout << "Enter index: ";
			std::cin >> index;
			std::cout << "Enter value: ";
			std::cin >> value;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				arrIA.insert_byIndex(value, index);
				timerStop = std::chrono::steady_clock::now();

				displayArray();
				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const MZ_ia::failure& fail)
			{
				addNotification(fail.what());
				system("cls");
			}
			break;
		case 2:
			displayArray();

			std::cout << "Enter index: ";
			std::cin >> index;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				arrIA.pop_index(index);
				timerStop = std::chrono::steady_clock::now();

				displayArray();
				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const MZ_ia::failure& fail)
			{
				addNotification(fail.what());
				system("cls");
			}

			break;
		case 3:
			displayArray();

			std::cout << "Enter value: ";
			std::cin >> value;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				index = arrIA.find(value);

				if (index < 0)
				{
					addNotification("The array element was not found");
					system("cls");
					break;
				}

				arrIA.pop_index(index);
				timerStop = std::chrono::steady_clock::now();

				displayArray();
				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const MZ_ia::failure& fail)
			{
				addNotification(fail.what());
				system("cls");
			}

			break;
		case 4:
			displayArray();

			std::cout << "Enter index: ";
			std::cin >> index;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				value = arrIA[index];
				timerStop = std::chrono::steady_clock::now();

				std::cout << "Value at index: " << value << std::endl;

				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const MZ_ia::failure& fail)
			{
				addNotification(fail.what());
				system("cls");
			}

			break;
		case 5:
			displayArray();

			std::cout << "Enter value: ";
			std::cin >> value;

			index = arrIA.find(value);

			if (index < 0)
			{
				addNotification("The array element was not found");
				system("cls");
				break;
			}
			else
				std::cout << "Value index: " << index << std::endl;

			displayClsWithPause();
			break;
		default:
			return;
			break;
		}
	}
}



void practicalWork2::menu_DLList() {
	
	int choise;
	while (true)
	{
		displayNotifications();
		
		std::cout << "Practical work 2: \n"
			<< "1 - Create an array according to the entered size. Then fill in with random numbers.\n"
			<< "2 - Create an array based on the entered elements. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (enter on one line))\n"
			<< "3 - Create an array of read elements from a file. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (must be written in one line))\n"
			<< "4 - Toggle timing. (current state = " << (bTimingIsEnabled ? "enabled" : "disabled") << ")\n"
			<< "5 - Work with an array\n"
			<< "6 - Back\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:

			displayClsWithPause();
			break;
		case 2:

			displayClsWithPause();
			break;
		case 3:

			displayClsWithPause();
			break;
		case 4:

			displayClsWithPause();
			break;
		case 5:

			displayClsWithPause();
			break;
		default:
			return;
			break;
		}
	}
}
