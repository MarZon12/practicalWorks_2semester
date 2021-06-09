#include <iostream>

#include "practicalWork1.h"
#include "practicalWork2.h"
#include "practicalWork3.h"
#include "CourseWork.h"

int main()
{
	practicalWork1 pw1;
	practicalWork2 pw2;
	practicalWork3 pw3;
	CourseWork cw;
	BinaryTree binTree;

	int choise;

	while (true)
	{
		std::cout << "Practical works\n";


		std::cout << "1 - Practical work 1\n"
			<< "2 - Practical work 2\n"
			<< "3 - Practical work 3\n"
			<< "4 - Course work\n"
			<< "5 - Exit\n";

		std::cout << "Your choice: ";
		std::cin >> choise;

		system("cls");
		switch (choise)
		{
		case 1:
			try
			{
				pw1.menu_main();
			}
			catch (const std::exception& youAreIdiot)
			{
				std::cout << "Something went wrong: " << youAreIdiot.what() << std::endl;
			}
			break;

		case 2:
			try
			{
				pw2.menu_main();
			}
			catch (const std::exception& youAreIdiot)
			{
				std::cout << "Something went wrong: " << youAreIdiot.what() << std::endl;
			}
			break;

		case 3:
			try
			{
				pw3.menu_main();
			}
			catch (const std::exception& youAreIdiot)
			{
				std::cout << "Something went wrong: " << youAreIdiot.what() << std::endl;
			}
			break;

		case 4:
			try
			{
				cw.menu_main();
			}
			catch (const std::exception& youAreIdiot)
			{
				std::cout << "Something went wrong: " << youAreIdiot.what() << std::endl;
			}
			break;

		default:
			return 0;
			break;
		}
		system("cls");
	}

	return 0;
}