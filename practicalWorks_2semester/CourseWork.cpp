#include "CourseWork.h"


//*******************//
//      PUBLIC       //
//*******************//


CourseWork::CourseWork() {
	useNullWhenPrint = false;
	bTimingIsEnabled = true;
	fileName = "file.txt";
};

//       -MENU-       //

void CourseWork::menu_main() {
	int choise,
		pos = 0;

	std::string inputString;

	extractIntFromStringResult extractionResult;

	std::ifstream fin;

	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerStop;

	srand(time(NULL));

	while (true)
	{
		displayNotifications();

		std::cout << "Course work: Binary search tree\n"
			<< "1 - Create a tree according to the entered size. Then fill in with random numbers.\n"
			<< "2 - Create a tree based on the entered elements. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (enter on one line))\n"
			<< "3 - Create a tree of read elements from a file. (the size is determined automatically; example: 12 2 20 1 2442 1 42 (must be written in one line))\n"
			<< "4 - Toggle \"use Null when displaying tree\". (current state = " << (useNullWhenPrint ? "enabled" : "disabled") << ")\n"
			<< "5 - Toggle timing. (current state = " << (bTimingIsEnabled ? "enabled" : "disabled") << ")\n"
			<< "6 - Work with an tree\n"
			<< "7 - Back\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:
			std::cout << "Enter tree size: ";
			std::cin >> choise;

			binTree.eraseTree();

			timerStart = std::chrono::steady_clock::now();

			for (size_t i = 0; i < choise; i++)
				binTree.insert(rand() % (99 - (-99) + 1) + (-99));

			timerStop = std::chrono::steady_clock::now();

			binTree.printTree(useNullWhenPrint);

			displayTimerResultWithCheck(timerStart, timerStop);

			displayClsWithPause();
			break;

		case 2:
			std::cout << "Enter the elements of the tree (in one line): ";

			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			std::getline(std::cin, inputString);

			binTree.eraseTree();

			pos = 0;
			timerStart = std::chrono::steady_clock::now();
			do
			{
				extractionResult = extractIntFromString(inputString, pos);
				if (extractionResult.success)
				{
					binTree.insert(extractionResult.intValue);
					pos = extractionResult.nextCharAfterIntPos;
				}
			} while (extractionResult.success);
			timerStop = std::chrono::steady_clock::now();

			binTree.printTree(useNullWhenPrint);

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

			binTree.eraseTree();

			pos = 0;
			timerStart = std::chrono::steady_clock::now();
			do
			{
				extractionResult = extractIntFromString(inputString, pos);
				if (extractionResult.success)
				{
					binTree.insert(extractionResult.intValue);
					pos = extractionResult.nextCharAfterIntPos;
				}
			} while (extractionResult.success);
			timerStop = std::chrono::steady_clock::now();

			fin.close();

			binTree.printTree(useNullWhenPrint);

			displayTimerResultWithCheck(timerStart, timerStop);

			displayClsWithPause();
			break;

		case 4:
			useNullWhenPrint = !useNullWhenPrint;
			addNotification("\"Use Null when displaying tree\" state has been updated");
			break;

		case 5:
			bTimingIsEnabled = !bTimingIsEnabled;
			addNotification("Timing state has been updated");
			break;

		case 6:
			menu_WorkWithTree();
			break;

		default:
			return;
			break;
		}
	}
}

void CourseWork::menu_WorkWithTree() {

	int choise,
		value;

	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerStop;

	BinaryTree::Node* tmpNode;

	srand(time(NULL));

	while (true)
	{
		displayNotifications();

		std::cout << "Course work: work with a tree\n"
			<< "1 - Insert\n"
			<< "2 - Remove item in tree (by value)\n"
			<< "3 - Get the Node of an element of a tree (by value, impossible to interact)\n"
			<< "4 - Pre-order travers\n"
			<< "5 - In-order travers\n"
			<< "6 - Post-order travers\n"
			<< "7 - Display tree\n"
			<< "8 - Back\n";
		std::cin >> choise;

		system("cls");

		switch (choise)
		{
		case 1:
			std::cout << "Enter value: ";
			std::cin >> value;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				binTree.insert(value);
				timerStop = std::chrono::steady_clock::now();

				binTree.printTree(useNullWhenPrint);
				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const std::exception& fail)
			{
				addNotification(fail.what());
				system("cls");
			}
			break;

		case 2:
			binTree.printTree(useNullWhenPrint);

			std::cout << "Enter value: ";
			std::cin >> value;

			try
			{
				timerStart = std::chrono::steady_clock::now();
				binTree.erase(value);
				timerStop = std::chrono::steady_clock::now();

				binTree.printTree(useNullWhenPrint);
				displayTimerResultWithCheck(timerStart, timerStop);

				displayClsWithPause();
			}
			catch (const std::exception& fail)
			{
				addNotification(fail.what());
				system("cls");
			}
			break;

		case 3:
			binTree.printTree(useNullWhenPrint);

			std::cout << "Enter value: ";
			std::cin >> value;

			timerStart = std::chrono::steady_clock::now();
			tmpNode = binTree.search(value);
			timerStop = std::chrono::steady_clock::now();

			if (tmpNode == nullptr)
				std::cout << "Not found!" << std::endl;
			else
				std::cout << "Found!" << std::endl;

			displayTimerResultWithCheck(timerStart, timerStop);

			displayClsWithPause();
			break;

		case 4:
			binTree.preOrderTravers();

			displayClsWithPause();
			break;

		case 5:
			binTree.inOrderTravers();

			displayClsWithPause();
			break;

		case 6:
			binTree.postOrderTravers();

			displayClsWithPause();
			break;
		case 7:
			binTree.printTree(useNullWhenPrint);

			displayClsWithPause();
			break;
		default:
			return;
			break;
		}
	}
}


//********************//
//      PRIVATE       //
//********************//


//       -NOTIFICATIONS-       //

void CourseWork::addNotification(std::string notification) {
	notificationsToDisplay.push_back(notification);
};

void CourseWork::displayNotifications() {
	if (!notificationsToDisplay.empty())
	{
		for (int i = 0; i < notificationsToDisplay.size(); i++)
			std::cout << "Notification: " << notificationsToDisplay[i] << std::endl;
		notificationsToDisplay.clear();
	}
}


//       -OTHER STUFF-       //

//return pos - first character after int found in string
CourseWork::extractIntFromStringResult CourseWork::extractIntFromString(std::string stringToAnalyze, int pos) {
	if (pos >= stringToAnalyze.length() || pos < 0)
		return extractIntFromStringResult();
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
		return extractIntFromStringResult(tempValue, true, tempPos + tempString.length() + pos);
	else
		return extractIntFromStringResult();
};

void CourseWork::displayTimerResultWithCheck(const std::chrono::steady_clock::time_point& timerStart, const std::chrono::steady_clock::time_point& timerStop) {
	if (bTimingIsEnabled)
		std::cout << "Time taken to complete: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timerStop - timerStart).count() << " ns" << std::endl;
}

void CourseWork::displayClsWithPause() {
	system("pause");
	system("cls");
}

