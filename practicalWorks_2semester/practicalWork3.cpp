#include "practicalWork3.h"



//*******************//
//      PUBLIC       //
//*******************//


//       -MENU-       //

void practicalWork3::menu_main() {
	int choise;

	std::string input;

	std::string nativeNotation;
	std::string standartNotation;
	std::string polishNotation;
	std::string reversePolishNotation;
	long double value;



	while (true)
	{
		displayNotifications();

		std::cout << "Practical work 3\n";


		std::cout << "1 - Enter notation\n"
			<< "2 - Get all information about notaion\n"
			<< "3 - Enter variables\n"
			<< "4 - Exit\n";

		std::cout << "Your choice: ";
		std::cin >> choise;

		system("cls");
		switch (choise)
		{
		case 1:
			std::cout << "Enter notation: ";

			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());

			getline(std::cin, input);

			try
			{
				notation.setNativeNotation(input);
				addNotification("Notation set!");
			}
			catch (const std::exception& youAreIdiot)
			{
				addNotification(std::string("Sorry, something went wrong: ") + youAreIdiot.what());
			}
			break;

		case 2:
			try
			{
				nativeNotation = PolishNotation::notationToString(notation.getNativeNotation());
				standartNotation = PolishNotation::notationToString(notation.getStandartNotation());
				polishNotation = PolishNotation::notationToString(notation.getPolishNotation());
				reversePolishNotation = PolishNotation::notationToString(notation.getReversePolishNotation());
				value = notation.getCalculatedNotation();

				std::cout << "Native notation: " << nativeNotation << std::endl;
				std::cout << "Native notation type: " << notation.getNativeNotationTypeStr() << std::endl << std::endl;

				std::cout << "Standart notation: " << standartNotation << std::endl;
				std::cout << "Polish notation: " << polishNotation << std::endl;
				std::cout << "Reverse polish notation: " << reversePolishNotation << std::endl << std::endl;

				std::cout << "Calculated: " << value << std::endl;
				system("pause");
				system("cls");
			}
			catch (const std::exception& youAreIdiot)
			{
				addNotification(std::string("Sorry, something went wrong: ") + youAreIdiot.what());
			}
			break;
		case 3:
			while (true)
			{
				PolishNotation::StateWithMessage msg(notation.getNameFirstUnsetVariable());
				long double inputDouble;
				if (msg.state == true)
				{
					std::cout << "Enter the value of variable " << msg.message << ": ";
					std::cin >> inputDouble;
					notation.setFirstUnsetVariable(inputDouble);
				}
				else
				{
					addNotification("All variables are set");
					break;
				}
			}
			break;

		default:
			return;
			break;
		}
		system("cls");
	}
}



//********************//
//      PRIVATE       //
//********************//


//       -NOTIFICATIONS-       //

void practicalWork3::addNotification(std::string notification) {
	notificationsToDisplay.push_back(notification);
};

void practicalWork3::displayNotifications() {
	if (!notificationsToDisplay.empty())
	{
		for (int i = 0; i < notificationsToDisplay.size(); i++)
			std::cout << "Notification: " << notificationsToDisplay[i] << std::endl;
		notificationsToDisplay.clear();
	}
}
