#include "practicalWork1.h"


practicalWork1::practicalWork1() {}
practicalWork1::~practicalWork1() {}

//Menus
void practicalWork1::menu_main() {
	int choise = 0;


	while (true)
	{
		loadData();
		displayNotifications();

		std::cout << "Practical work 1\n" <<
			"1 - Add new student\n" <<
			"2 - Edit student data\n" <<
			"3 - Display all data about students\n" <<
			"4 - Display information about all students in group N\n" <<
			"5 - Display of the top of the most successful students with the highest rating average score for the last session.\n" <<
			"6 - Display of the number of male and female students.\n" <<
			"7 - Display the number of students who will receive the scholarship\n" <<
			"8 - Display data on students who do not receive a scholarship\n" <<
			"9 - Display data on students who study only \"good\" and \"excellent\"\n" <<
			"10 - Display data on students who study only \"excellent\"\n" <<
			"11 - Display data about students with a number in the list - k\n" <<
			"12 - Displays all entries made on the entered day\n" <<
			"13 - Exit" << std::endl;
		std::cin >> choise;

		system("cls");
		switch (choise)
		{
		case 1:
			menu_addNewStudent();
			break;

		case 2:
			menu_editStudentData();
			break;

		case 3:
			displayAllStudents();
			break;

		case 4:
			menu_groupSelect();
			break;

		case 5:
			displayTopStudentsOfThePreviousSession();
			break;

		case 6:
			displayManAndWomanCount();
			break;

		case 7:
			displayCountOfStudentReceivingScholarships();
			break;

		case 8:
			displayStudentNotReceivingScholarships();
			break;

		case 9:
			displayStudentsByGrades(4);
			break;

		case 10:
			displayStudentsByGrades(5);
			break;

		case 11:
			std::cout << "Enter student sequential number in group: ";
			std::cin >> choise;
			displayStudentsBySequentialNumberInGroupList(choise);
			break;

		case 12:
			menu_daySelect();
			break;
		default:
			return;
			break;
		}
	}
}

void practicalWork1::menu_groupSelect() {
	displayNotifications();
	int choise,
		groupIndex;
	bool found = false;
	std::cout << "Enter group number: ";
	std::cin >> choise;
	for (int i = 0; i < groupsData.size(); i++)
	{
		if (groupsData[i].groupNumber == choise)
		{
			found = true;
			groupIndex = i;
			break;
		}
	}

	system("cls");
	if (found)
		displayAllStudentsOfGroup(groupIndex);
	else
		addNotification("Group not found");
}

void practicalWork1::menu_addNewStudent() {
	displayNotifications();

	student tempStudentData;
	int groupNumber;
	formOfEducation formOfEducation_;

	short choise;

	std::cout << "Enter the data according to the example below:\n" <<
		"Full name: Martynov Alexander Eduardovich\n" <<
		"Man or woman? (man - 0, woman - 1): 0\n" <<
		"Group number: 323\n" <<
		"Sequential number in the group list: 12\n" <<
		"Grades for the last session (first 3 grades for exams, and then 5 grades for differentiated tests): 3 3 3 5 5 5 5 5\n" <<
		"Form of education(fulltime - 0, parttime - 1, extramural - 2): 1\n" << std::endl;

	std::cout << "Full name: ";
	clearCin();
	std::getline(std::cin, tempStudentData.fullName);

	std::cout << "Man or woman? (man - 0, woman - 1): ";
	std::cin >> tempStudentData.bIsWoman;

	std::cout << "Group number: ";
	std::cin >> groupNumber;

	std::cout << "Sequential number in the group list: ";
	std::cin >> tempStudentData.sequentialNumberInGroupList;

	std::cout << "Grades for the last session (first 3 grades for exams, and then 5 grades for differentiated tests): ";
	for (short i = 0; i < 3; i++)
		std::cin >> tempStudentData.lastSessionGrades.exam[i];
	for (short i = 0; i < 5; i++)
		std::cin >> tempStudentData.lastSessionGrades.diffTest[i];

	//WARNING добавить проверку существубщей группы по номеру для автозаполнения
	std::cout << "Form of education(fulltime - 0, parttime - 1, extramural - 2): ";
	std::cin >> choise;
	switch (choise)
	{
		case static_cast<int>(formOfEducation::FULLTIME) :
			formOfEducation_ = formOfEducation::FULLTIME;
			break;

			case static_cast<int>(formOfEducation::PARTTIME) :
				formOfEducation_ = formOfEducation::PARTTIME;
				break;

				case static_cast<int>(formOfEducation::EXTRAMURAL) :
					formOfEducation_ = formOfEducation::EXTRAMURAL;
					break;
				default:
					addNotification("A non-existent form of education was detected. The default form of education was set - full-time.");
					formOfEducation_ = formOfEducation::FULLTIME;
					break;
	}

	tempStudentData.timeOfLastChange = time(NULL);
	tempStudentData.bCanReceiveScholarship = canStudentReceiveScholarship(tempStudentData.lastSessionGrades, formOfEducation_);

	addNewStudent(tempStudentData, formOfEducation_, groupNumber);

	saveData();
	system("cls");
}

void practicalWork1::menu_editStudentData() {
	displayNotifications();

	if (groupsData.empty())
	{
		addNotification("No group created yet");
		return;
	}

	int choise;

	student tempStudentData;
	int groupNumber;
	formOfEducation formOfEducation_;

	groupData* studentGroupDataRef;
	student* studentDataRef;

	std::cout << "Select a group:\n";
	for (int i = 0; i < groupsData.size(); i++)
		std::cout << i << " - " << groupsData[i].groupNumber << std::endl;

	std::cin >> choise;
	//wrong group
	if (choise >= groupsData.size())
	{
		addNotification("You have selected the wrong group");
		system("cls");
		return;
	}
	studentGroupDataRef = &groupsData[choise];

	system("cls");


	std::cout << "Select student:\n";
	for (int i = 0; i < studentGroupDataRef->students.size(); i++)
		std::cout << i << " - " << studentGroupDataRef->students[i].fullName << std::endl;

	std::cin >> choise;
	//wrong student
	if (choise >= studentGroupDataRef->students.size())
	{
		addNotification("You have selected the wrong student");
		system("cls");
		return;
	}
	studentDataRef = &studentGroupDataRef->students[choise];

	system("cls");


	std::cout << "Change the student information (if you want to keep the previous one, enter \"-1\"):\n";

	std::cout << "Full name: ";
	clearCin();
	std::getline(std::cin, tempStudentData.fullName);

	std::cout << "Man or woman? (man - 0, woman - 1; entering \"-1\" is prohibited): ";
	std::cin >> tempStudentData.bIsWoman;

	std::cout << "Group number: ";
	std::cin >> groupNumber;

	std::cout << "Sequential number in the group list: ";
	std::cin >> tempStudentData.sequentialNumberInGroupList;

	std::cout << "Grades for the last session (first 3 grades for exams, and then 5 grades for differentiated tests; example: 3 -1 3 5 5 5 5 -1): ";
	for (short i = 0; i < 3; i++)
		std::cin >> tempStudentData.lastSessionGrades.exam[i];
	for (short i = 0; i < 5; i++)
		std::cin >> tempStudentData.lastSessionGrades.diffTest[i];

	//WARNING добавить проверку существубщей группы по номеру для автозаполнения
	std::cout << "Form of education(fulltime - 0, parttime - 1, extramural - 2; entering \"-1\" is prohibited): ";
	std::cin >> choise;
	switch (choise)
	{
		case static_cast<int>(formOfEducation::FULLTIME) :
			formOfEducation_ = formOfEducation::FULLTIME;
			break;

			case static_cast<int>(formOfEducation::PARTTIME) :
				formOfEducation_ = formOfEducation::PARTTIME;
				break;

				case static_cast<int>(formOfEducation::EXTRAMURAL) :
					formOfEducation_ = formOfEducation::EXTRAMURAL;
					break;

				default:
					addNotification("A non-existent form of education was detected. The default form of education was set - full-time.");
					formOfEducation_ = formOfEducation::FULLTIME;
					break;
	}

	tempStudentData.timeOfLastChange = time(NULL);
	tempStudentData.bCanReceiveScholarship = canStudentReceiveScholarship(tempStudentData.lastSessionGrades, formOfEducation_);

	editStudentData(studentDataRef, studentGroupDataRef, tempStudentData, groupData(groupNumber, formOfEducation_));
	saveData();
	system("cls");
}

void practicalWork1::menu_daySelect() {
	int day, month, year;
	int partOfTheDayChoise;

	tm rTime;
	tm minTime,
		maxTime;

	char timeStr[26]{ 0 };


	std::cout << "Type day, month, year (ex. 18 2 2021): ";
	std::cin >> day >> month >> year;
	rTime.tm_year = year - 1900;
	rTime.tm_mon = month - 1;
	rTime.tm_mday = day;
	rTime.tm_hour = 0;
	rTime.tm_min = 0;
	rTime.tm_sec = 0;
	rTime.tm_isdst = -1;

	std::cout << "Choose part of the day (pm (00:00-12:00) = 0, am(12:00:00-23:59:59) = 1, day (00:00:00-23:59:59) = 2): ";
	std::cin >> partOfTheDayChoise;

	switch (partOfTheDayChoise)
	{
	case 0:
		minTime = rTime;
		maxTime.tm_year = year - 1900;
		maxTime.tm_mon = month - 1;
		maxTime.tm_mday = day;
		maxTime.tm_hour = 12;
		maxTime.tm_min = 0;
		maxTime.tm_sec = 0;
		maxTime.tm_isdst = -1;
		break;
	case 1:
		minTime.tm_year = year - 1900;
		minTime.tm_mon = month - 1;
		minTime.tm_mday = day;
		minTime.tm_hour = 12;
		minTime.tm_min = 0;
		minTime.tm_sec = 0;
		minTime.tm_isdst = -1;

		maxTime.tm_year = year - 1900;
		maxTime.tm_mon = month - 1;
		maxTime.tm_mday = day;
		maxTime.tm_hour = 23;
		maxTime.tm_min = 59;
		maxTime.tm_sec = 59;
		maxTime.tm_isdst = -1;
		break;
	default:
		minTime = rTime;
		maxTime.tm_year = year - 1900;
		maxTime.tm_mon = month - 1;
		maxTime.tm_mday = day;
		maxTime.tm_hour = 23;
		maxTime.tm_min = 59;
		maxTime.tm_sec = 59;
		maxTime.tm_isdst = -1;
		break;
	}

	system("cls");
	displayByTimeOfLastChange(mktime(&minTime), mktime(&maxTime));

}

//PRIVATE

//Notifications
void practicalWork1::addNotification(std::string notification) {
	notificationsToDisplay.push_back(notification);
};
void practicalWork1::displayNotifications() {
	if (!notificationsToDisplay.empty())
	{
		for (int i = 0; i < notificationsToDisplay.size(); i++)
			std::cout << "Notification: " << notificationsToDisplay[i] << std::endl;
		notificationsToDisplay.clear();
	}
}



//Instruments
void practicalWork1::clearCin() {
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
}

bool practicalWork1::canStudentReceiveScholarship(const sessionGrades& studentLastSessionGrades, const formOfEducation& studentFormOfEducation) {
	if (studentFormOfEducation == formOfEducation::EXTRAMURAL || studentFormOfEducation == formOfEducation::PARTTIME)
		return false;
	for (int i = 0; i < 3; i++)
		if (studentLastSessionGrades.exam[i] < 4)
			return false;
	for (int i = 0; i < 5; i++)
		if (studentLastSessionGrades.diffTest[i] < 4)
			return false;
	return true;
}

std::string practicalWork1::formOfEducationToString(formOfEducation formOfEducation_) {
	switch (formOfEducation_)
	{
	case formOfEducation::FULLTIME:
		return "Full-Time";
		break;
	case formOfEducation::PARTTIME:
		return "Part-Time";
		break;
	case formOfEducation::EXTRAMURAL:
		return "Extramural";
		break;
	default:
		return "ERROR";
		break;
	}
}

void practicalWork1::displayClsWithPause() {
	system("pause");
	system("cls");
}



//Saving and loading
bool practicalWork1::saveData() {
	std::ofstream fout;
	saveMarkers sMarkers;

	fout.open(sMarkers.saveName);
	if (!fout.is_open()) {
		addNotification("Could not open save file for save");
		return false;
	}

	fout << sMarkers.saveVersion << sMarkers.version << std::endl;

	for (int i = 0; i < groupsData.size(); i++)
	{
		fout << sMarkers.groupData << std::endl;
		fout << sMarkers.GROUP_Number << groupsData[i].groupNumber << std::endl;
		fout << sMarkers.GROUP_formOfEducationCode << static_cast<int>(groupsData[i].groupFormOfEducation) << std::endl;
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			fout << sMarkers.studentData << std::endl;
			fout << sMarkers.STUDENT_fullName << groupsData[i].students[j].fullName << std::endl;
			fout << sMarkers.STUDENT_sexCode << groupsData[i].students[j].bIsWoman << std::endl;
			fout << sMarkers.STUDENT_sequentialNumberInGroupList << groupsData[i].students[j].sequentialNumberInGroupList << std::endl;
			//Grades
			fout << sMarkers.STUDENT_lastSessionGrades;
			for (int k = 0; k < 3; k++) {
				fout << short(groupsData[i].students[j].lastSessionGrades.exam[k]) << ' ';
			}
			for (int k = 0; k < 5; k++) {
				fout << groupsData[i].students[j].lastSessionGrades.diffTest[k];
				if (k == 4)
					fout << std::endl;
				else
					fout << ' ';
			}
			//fout << "CanReceiveScholarship: " << groupsData[i].students[j].bCanReceiveScholarship << std::endl;
			fout << sMarkers.STUDENT_timeOfLastChange << groupsData[i].students[j].timeOfLastChange << std::endl;
		}
	}
	fout.close();
	addNotification("Successfully saved to save file");
	return true;
}

//pos - the first character after the word indicating the data type in the string
practicalWork1::LD_DTIS_withPos practicalWork1::LD_getDataTypeInString(const std::string& stringToAnalyze) {
	size_t pos;
	saveMarkers sMarkers;

	pos = stringToAnalyze.find(sMarkers.saveVersion);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::SAVEVERSION, pos + sMarkers.saveVersion.length());

	pos = stringToAnalyze.find(sMarkers.groupData);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::NEW_GROUP, pos + sMarkers.groupData.length());

	pos = stringToAnalyze.find(sMarkers.studentData);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::NEW_STUDENT, pos + sMarkers.studentData.length());

	//group data
	pos = stringToAnalyze.find(sMarkers.GROUP_Number);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::GROUP_NUMBER, pos + sMarkers.GROUP_Number.length());

	pos = stringToAnalyze.find(sMarkers.GROUP_formOfEducationCode);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::GROUP_FORMOFEDUCATIONCODE, pos + sMarkers.GROUP_formOfEducationCode.length());

	//student data
	pos = stringToAnalyze.find(sMarkers.STUDENT_fullName);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::STUDENT_FULLNAME, pos + sMarkers.STUDENT_fullName.length());

	pos = stringToAnalyze.find(sMarkers.STUDENT_sexCode);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::STUDENT_SEXCODE, pos + sMarkers.STUDENT_sexCode.length());

	pos = stringToAnalyze.find(sMarkers.STUDENT_sequentialNumberInGroupList);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::STUDENT_SEQUENTIALNUMBERIBGROUPLIST, pos + sMarkers.STUDENT_sequentialNumberInGroupList.length());

	pos = stringToAnalyze.find(sMarkers.STUDENT_lastSessionGrades);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::STUDENT_LASTSESSIONGRADES, pos + sMarkers.STUDENT_lastSessionGrades.length());

	pos = stringToAnalyze.find(sMarkers.STUDENT_timeOfLastChange);
	if (pos != std::string::npos)
		return LD_DTIS_withPos(LD_dataTypeInString::STUDENT_TIMEOFLASTCHANGE, pos + sMarkers.STUDENT_timeOfLastChange.length());

	//if UNKNOWN data type
	return LD_DTIS_withPos(LD_dataTypeInString::UNKNOWN, std::string::npos);
}
//pos - first character after int found in string
practicalWork1::LD_getNextIntFromStringResult practicalWork1::LD_getNextIntFromString(std::string stringToAnalyze, int pos) {
	if (pos >= stringToAnalyze.length() || pos < 0)
		return LD_getNextIntFromStringResult();
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
		return LD_getNextIntFromStringResult(tempValue, true, tempPos + tempString.length() + pos);
	else
		return LD_getNextIntFromStringResult();
};

bool practicalWork1::loadData() {
	std::string stringToAnalyze;

	LD_DTIS_withPos dataTypeCodeWithPos;
	LD_getNextIntFromStringResult getNextIntResult;
	saveMarkers sMarkers;

	int activeGroupIndex = -1,
		activeStudentIndex = -1;
	bool processingSessionGrades = false;

	std::ifstream fin;

	fin.open(sMarkers.saveName);
	if (!fin.is_open())
	{
		addNotification("Could not open save file for load");
		return false;
	}

	//pre-loading
	groupsData.clear();

	while (std::getline(fin, stringToAnalyze))
	{
		dataTypeCodeWithPos = LD_getDataTypeInString(stringToAnalyze);
		switch (dataTypeCodeWithPos.dataType)
		{
		case LD_dataTypeInString::UNKNOWN:
			addNotification("Unknown data string found in save file:");
			addNotification(stringToAnalyze);
			break;

		case LD_dataTypeInString::SAVEVERSION:
			//mb in the future (never comes) (to enable legacy support in the save loader)
			break;

		case LD_dataTypeInString::NEW_GROUP:
			groupsData.push_back(groupData());
			activeGroupIndex = groupsData.size() - 1;
			break;

		case LD_dataTypeInString::NEW_STUDENT:
			if (activeGroupIndex >= 0)
			{
				groupsData[activeGroupIndex].students.push_back(student());
				activeStudentIndex = groupsData[activeGroupIndex].students.size() - 1;
			}
			else
			{
				groupsData.push_back(groupData());
				activeGroupIndex = groupsData.size() - 1;
				groupsData[activeGroupIndex].students.push_back(student());
				activeStudentIndex = groupsData[activeGroupIndex].students.size() - 1;
			}
			break;

		case LD_dataTypeInString::GROUP_NUMBER:
			if (activeGroupIndex >= 0)
			{
				getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
				if (getNextIntResult.success)
					groupsData[activeGroupIndex].groupNumber = getNextIntResult.intValue;
				else
					addNotification("[loader] An error occurred while processing group number");
			}
			break;

		case LD_dataTypeInString::GROUP_FORMOFEDUCATIONCODE:
			if (activeGroupIndex >= 0)
			{
				getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
				if (getNextIntResult.success)
					groupsData[activeGroupIndex].groupFormOfEducation = static_cast<formOfEducation>(getNextIntResult.intValue);
				else
					addNotification("[loader] An error occurred while processing group form of education");
			}
			break;

		case LD_dataTypeInString::STUDENT_FULLNAME:
			if (activeStudentIndex >= 0)
				groupsData[activeGroupIndex].students[activeStudentIndex].fullName = stringToAnalyze.substr(dataTypeCodeWithPos.pos);
			else {
				addNotification("[loader] An error occurred while processing student full name:");
				addNotification(stringToAnalyze);
			}
			break;

		case LD_dataTypeInString::STUDENT_SEXCODE:
			if (activeStudentIndex >= 0)
			{
				getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
				if (getNextIntResult.success)
					groupsData[activeGroupIndex].students[activeStudentIndex].bIsWoman = static_cast<bool>(getNextIntResult.intValue);
				else
					addNotification("[loader] An error occurred while processing student sex (code)");
			}
			break;

		case LD_dataTypeInString::STUDENT_SEQUENTIALNUMBERIBGROUPLIST:
			if (activeStudentIndex >= 0)
			{
				getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
				if (getNextIntResult.success)
					groupsData[activeGroupIndex].students[activeStudentIndex].sequentialNumberInGroupList = getNextIntResult.intValue;
				else
					addNotification("[loader] An error occurred while processing student sequential number");
			}
			break;

		case LD_dataTypeInString::STUDENT_LASTSESSIONGRADES:
			if (activeStudentIndex >= 0)
			{
				for (int i = 0; i < 3; i++)
				{
					if (!processingSessionGrades) {
						getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
						processingSessionGrades = true;
					}
					else
						getNextIntResult = LD_getNextIntFromString(stringToAnalyze, getNextIntResult.nextCharAfterIntPos);

					if (getNextIntResult.success)
						groupsData[activeGroupIndex].students[activeStudentIndex].lastSessionGrades.exam[i] = getNextIntResult.intValue;
					else
						addNotification("[loader] An error occurred while processing student last session grades (exam)");
				}
				for (int i = 0; i < 5; i++)
				{
					if (!processingSessionGrades) {
						getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
						processingSessionGrades = true;
					}
					else
						getNextIntResult = LD_getNextIntFromString(stringToAnalyze, getNextIntResult.nextCharAfterIntPos);
					if (getNextIntResult.success)
						groupsData[activeGroupIndex].students[activeStudentIndex].lastSessionGrades.diffTest[i] = getNextIntResult.intValue;
					else
						addNotification("[loader] An error occurred while processing student last session grades (diffTest)");
				}

				//updating scholarship
				groupsData[activeGroupIndex].students[activeStudentIndex].bCanReceiveScholarship = canStudentReceiveScholarship(groupsData[activeGroupIndex].students[activeStudentIndex].lastSessionGrades, groupsData[activeGroupIndex].groupFormOfEducation);

				processingSessionGrades = false;
			}
			break;

		case LD_dataTypeInString::STUDENT_TIMEOFLASTCHANGE:
			if (activeStudentIndex >= 0)
			{
				getNextIntResult = LD_getNextIntFromString(stringToAnalyze, dataTypeCodeWithPos.pos);
				if (getNextIntResult.success)
					groupsData[activeGroupIndex].students[activeStudentIndex].timeOfLastChange = getNextIntResult.intValue;
				else
					addNotification("[loader] An error occurred while processing student time of last change");
			}
			break;

		default:
			addNotification("[loader] Something went wrong: the data type in the string was detected incorrectly");
			break;
		}
	}
	fin.close();

	addNotification("Successfully loaded from save file");
	return true;
}






bool practicalWork1::ANS_checkStudentCanBeAddedInGroup(const groupData& possibleGroup, const student& studentData, const formOfEducation& studentFormOfEducation) {
	if (possibleGroup.groupFormOfEducation != studentFormOfEducation)
	{
		addNotification("The group for the student has been found, but the form of education the group does not match the form of education the student. It is not possible to add a student");
		return false;
	}

	if (possibleGroup.students.empty())
		return true;
	else
	{
		for (int i = 0; i < possibleGroup.students.size(); i++)
		{
			if (possibleGroup.students[i].fullName == studentData.fullName)
			{
				addNotification("This student has already been added. It is not possible to add a student");
				return false;
			}
			if (possibleGroup.students[i].sequentialNumberInGroupList == studentData.sequentialNumberInGroupList)
			{
				addNotification("The sequence number of the added student in the group coincides with the existing one. It is not possible to add a student");
				return false;
			}
		}
	}

	return true;
}

bool practicalWork1::addNewStudent(const student& studentToAdd, const formOfEducation& studentFormOfEducation, const int& studentGroupNumber) {
	bool bExistingGroupFound = false;

	if (!groupsData.empty())
	{
		for (int i = 0; i < groupsData.size(); i++)
		{
			if (groupsData[i].groupNumber == studentGroupNumber)
			{
				bExistingGroupFound = true;
				if (ANS_checkStudentCanBeAddedInGroup(groupsData[i], studentToAdd, studentFormOfEducation))
				{
					groupsData[i].students.push_back(studentToAdd);
					addNotification("The student was added successfully");
					return true;
				}
			}
		}
		if (!bExistingGroupFound)
		{
			groupsData.push_back(groupData(studentGroupNumber, studentFormOfEducation));
			groupsData[groupsData.size() - 1].students.push_back(studentToAdd);
			return true;
		}
	}
	else
	{
		groupsData.push_back(groupData(studentGroupNumber, studentFormOfEducation));
		groupsData[groupsData.size() - 1].students.push_back(studentToAdd);
		return true;
	}
}



// true = need to edit
bool practicalWork1::ESD_processData(int intToProcess) {
	if (intToProcess == -1)
		return false;
	return true;
}
// true = need to edit
bool practicalWork1::ESD_processData(const std::string& stringToProcess) {
	for (int i = 0; i < stringToProcess.size() - 1; i++)
	{
		if (stringToProcess[i] != ' ')
		{
			if (stringToProcess[i] == '-' && (static_cast<int>(stringToProcess[i + 1]) >= static_cast<int>('0') && static_cast<int>(stringToProcess[i + 1]) <= static_cast<int>('9')))
				return false;
		}
	}
	return true;
}
// always true: correcting finalSessionGradesRef
bool practicalWork1::ESD_processData(const sessionGrades& currentSessionGradesRef, const sessionGrades& newSessionGradesRef, sessionGrades& finalSessionGradesRef) {
	for (short i = 0; i < 3; i++)
		if (newSessionGradesRef.exam[i] == -1)
			finalSessionGradesRef.exam[i] = currentSessionGradesRef.exam[i];
		else
			finalSessionGradesRef.exam[i] = newSessionGradesRef.exam[i];
	for (short i = 0; i < 5; i++)
		if (newSessionGradesRef.diffTest[i] == -1)
			finalSessionGradesRef.diffTest[i] = currentSessionGradesRef.diffTest[i];
		else
			finalSessionGradesRef.diffTest[i] = newSessionGradesRef.diffTest[i];
	return true;
}
// -1 if not existing
int practicalWork1::ESD_getGroupIndex(int groupNumber) {
	for (int i = 0; i < groupsData.size(); i++)
		if (groupsData[i].groupNumber == groupNumber)
			return i;
	return -1;
}
// true is success
bool practicalWork1::ESD_removeStudentFromGroup(groupData& groupDataRef, student& studentDataRef) {
	int groupIndex = ESD_getGroupIndex(groupDataRef.groupNumber);
	if (groupIndex >= 0)
	{
		for (int i = 0; i < groupsData[groupIndex].students.size(); i++)
		{
			if (groupsData[groupIndex].students[i] == studentDataRef)
			{
				for (int j = i; j < groupsData[groupIndex].students.size() - 1; j++)
					groupsData[groupIndex].students[j] = groupsData[groupIndex].students[j + 1];
				groupsData[groupIndex].students.pop_back();
				return true;
			}
		}
		addNotification("Unable to delete student: student not found.");
		return false;
	}
	else {
		addNotification("Unable to delete student: the group does not exist");
		return false;
	}
}

void practicalWork1::editStudentData(student* studentDataRef, groupData* studentGroupDataRef, student& newStudentDataRef, groupData newStudentGroupData) {
	student finalStudentData(studentDataRef->fullName, studentDataRef->bIsWoman, studentDataRef->sequentialNumberInGroupList, studentDataRef->lastSessionGrades);
	groupData finalStudentGroupData(studentGroupDataRef->groupNumber, studentGroupDataRef->groupFormOfEducation);

	//Student
	if (ESD_processData(newStudentDataRef.fullName))
		finalStudentData.fullName = newStudentDataRef.fullName;

	if (ESD_processData(newStudentDataRef.sequentialNumberInGroupList))
		finalStudentData.sequentialNumberInGroupList = newStudentDataRef.sequentialNumberInGroupList;

	ESD_processData(studentDataRef->lastSessionGrades, newStudentDataRef.lastSessionGrades, finalStudentData.lastSessionGrades);

	//Group
	if (ESD_processData(newStudentGroupData.groupNumber))
		finalStudentGroupData.groupNumber = newStudentGroupData.groupNumber;

	if (finalStudentGroupData.groupFormOfEducation != newStudentGroupData.groupFormOfEducation)
		finalStudentGroupData.groupFormOfEducation = newStudentGroupData.groupFormOfEducation;

	finalStudentData.timeOfLastChange = time(NULL);
	finalStudentData.bCanReceiveScholarship = canStudentReceiveScholarship(finalStudentData.lastSessionGrades, finalStudentGroupData.groupFormOfEducation);

	bool sequrntialNumberActualyExist = false;
	//Updating process
	if (finalStudentGroupData.groupNumber != studentGroupDataRef->groupNumber) {
		if (ESD_getGroupIndex(finalStudentGroupData.groupNumber) >= 0)
			if (addNewStudent(finalStudentData, finalStudentGroupData.groupFormOfEducation, finalStudentGroupData.groupNumber)) {
				ESD_removeStudentFromGroup(*studentGroupDataRef, *studentDataRef);
				addNotification("Student data updated successfully. The student was moved to a another group");
			}
	}
	else
	{
		if (finalStudentGroupData.groupFormOfEducation == studentGroupDataRef->groupFormOfEducation)
		{
			for (int i = 0; i < studentGroupDataRef->students.size(); i++)
			{
				if (finalStudentData.sequentialNumberInGroupList == studentGroupDataRef->students[i].sequentialNumberInGroupList)
				{
					if (studentDataRef->fullName != studentGroupDataRef->students[i].fullName)
					{
						sequrntialNumberActualyExist = true;
						break;
					}
				}
			}

			if (sequrntialNumberActualyExist)
				addNotification("Unable to update student information: new student sequence number matches another student's sequence number");
			else {
				*studentDataRef = finalStudentData;
				addNotification("Student data updated successfully");
			}
		}
		else
		{
			addNotification("Unable to update student information: the new education form does not match the education form specified for the group");
			return;
		}
	}
}



//reusable display functions
void practicalWork1::displayStudent(const student& studentRef) {
	char timeStr[26];
	std::cout << "Full name: " << studentRef.fullName << std::endl;
	std::cout << "Sex: " << (studentRef.bIsWoman ? "female" : "male") << std::endl;
	std::cout << "Sequential number in group list: " << studentRef.sequentialNumberInGroupList << std::endl;
	std::cout << "Last session grades (first 3 grades for exams, and then 5 grades for differentiated tests): ";
	for (int i = 0; i < 3; i++)
		std::cout << studentRef.lastSessionGrades.exam[i] << ' ';
	for (int i = 0; i < 5; i++)
		std::cout << studentRef.lastSessionGrades.diffTest[i] << ' ';
	std::cout << std::endl;
	std::cout << "Can receive scholarship: " << (studentRef.bCanReceiveScholarship ? "yes" : "no") << std::endl;
	ctime_s(timeStr, 26, &studentRef.timeOfLastChange);
	std::cout << "Time of last change: " << timeStr << std::endl;
}
void practicalWork1::displayGroupInfo(int groupIndex) {
	std::cout << "Group number: " << groupsData[groupIndex].groupNumber << std::endl;
	std::cout << "Group form of education: " << formOfEducationToString(groupsData[groupIndex].groupFormOfEducation) << std::endl;
}



void practicalWork1::displayAllStudents() {
	displayNotifications();

	for (int i = 0; i < groupsData.size(); i++)
	{
		displayGroupInfo(i);
		std::cout << "Students:" << std::endl;
		for (int j = 0; j < groupsData[i].students.size(); j++)
			displayStudent(groupsData[i].students[j]);
	}

	displayClsWithPause();
}

void practicalWork1::displayAllStudentsOfGroup(int groupIndex) {
	displayNotifications();

	displayGroupInfo(groupIndex);
	std::cout << "Students:" << std::endl;
	for (int j = 0; j < groupsData[groupIndex].students.size(); j++)
		displayStudent(groupsData[groupIndex].students[j]);

	displayClsWithPause();
}

void practicalWork1::performCombSort(const int studentsCount, int* studentstGroupIndex, int* studentsSelfIndex, int* studentsSelfSumGrade)
{
	double factor = 1.2473309;
	double step_sort = static_cast<double>(studentsCount - 1);
	int buffer;
	while (step_sort >= 1)
	{
		for (int i = 0; i + step_sort < studentsCount; i++)
		{
			if (studentsSelfSumGrade[i] > studentsSelfSumGrade[i + static_cast<int>(step_sort)])
			{
				buffer = studentsSelfSumGrade[i];
				studentsSelfSumGrade[i] = studentsSelfSumGrade[i + static_cast<int>(step_sort)];
				studentsSelfSumGrade[i + static_cast<int>(step_sort)] = buffer;

				buffer = studentstGroupIndex[i];
				studentstGroupIndex[i] = studentstGroupIndex[i + static_cast<int>(step_sort)];
				studentstGroupIndex[i + static_cast<int>(step_sort)] = buffer;

				buffer = studentsSelfIndex[i];
				studentsSelfIndex[i] = studentsSelfIndex[i + static_cast<int>(step_sort)];
				studentsSelfIndex[i + static_cast<int>(step_sort)] = buffer;
			}
		}
		step_sort /= factor;
	}

	//OptimizedMethod
	bool bSorted;
	int i = 0;
	do
	{
		bSorted = true;
		for (int y = 0; y < studentsCount - 1 - i; y++)
		{
			if (studentsSelfSumGrade[y] > studentsSelfSumGrade[y + 1])
			{
				buffer = studentsSelfSumGrade[y];
				studentsSelfSumGrade[y] = studentsSelfSumGrade[y + 1];
				studentsSelfSumGrade[y + 1] = buffer;

				buffer = studentstGroupIndex[y];
				studentstGroupIndex[y] = studentstGroupIndex[y + 1];
				studentstGroupIndex[y + 1] = buffer;

				buffer = studentsSelfIndex[y];
				studentsSelfIndex[y] = studentsSelfIndex[y + 1];
				studentsSelfIndex[y + 1] = buffer;

				bSorted = false;
			}
		}
		i++;
	} while (!bSorted);
}



int practicalWork1::getStudentsCount() {
	int studentsCount = 0;
	for (int i = 0; i < groupsData.size(); i++)
		studentsCount += groupsData[i].students.size();
	return studentsCount;
}

void practicalWork1::displayTopStudentsOfThePreviousSession() {
	displayNotifications();

	int studentsCount = 0,
		maxTopCount = 0;

	int* groupIndex = nullptr,
		* studentIndex = nullptr,
		* studentSumGrade = nullptr;

	studentsCount = getStudentsCount();

	groupIndex = new int[studentsCount] { 0 };
	studentIndex = new int[studentsCount] { 0 };
	studentSumGrade = new int[studentsCount] { 0 };

	int activeStudentSortIndex = 0,
		tempStudentSumGrade = 0;

	for (int i = 0; i < groupsData.size(); i++)
	{
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			groupIndex[activeStudentSortIndex] = i;
			studentIndex[activeStudentSortIndex] = j;

			for (int k = 0; k < 3; k++)
				tempStudentSumGrade += groupsData[i].students[j].lastSessionGrades.exam[k];
			for (int k = 0; k < 5; k++)
				tempStudentSumGrade += groupsData[i].students[j].lastSessionGrades.diffTest[k];

			studentSumGrade[activeStudentSortIndex] = tempStudentSumGrade;

			activeStudentSortIndex++;
			tempStudentSumGrade = 0;
		}
	}


	performCombSort(studentsCount, groupIndex, studentIndex, studentSumGrade);

	std::cout << "Top students by GPA:\n" << std::endl;


	if (studentsCount < 10)
		maxTopCount = studentsCount;
	else
		maxTopCount = 10;

	for (int i = maxTopCount - 1; i >= 0; i--)
	{
		std::cout << (maxTopCount - i);
		switch (maxTopCount - i)
		{
		case 1:
			std::cout << "st";
			break;
		case 2:
			std::cout << "nd";
			break;
		case 3:
			std::cout << "rd";
			break;
		default:
			std::cout << "th";
			break;
		}
		std::cout << " place:" << std::endl;

		displayGroupInfo(groupIndex[i]);
		displayStudent(groupsData[groupIndex[i]].students[studentIndex[i]]);
		std::cout << std::endl;
	}

	delete[] groupIndex;
	delete[] studentIndex;
	delete[] studentSumGrade;

	displayClsWithPause();
}

void practicalWork1::displayManAndWomanCount() {
	displayNotifications();

	int counterM = 0,
		counterF = 0;

	for (int i = 0; i < groupsData.size(); i++)
	{
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			if (groupsData[i].students[j].bIsWoman)
				counterF++;
			else
				counterM++;
		}
	}

	std::cout << "Number of male: " << counterM << std::endl;
	std::cout << "Number of female: " << counterF << std::endl;

	displayClsWithPause();
}

void practicalWork1::displayCountOfStudentReceivingScholarships() {
	displayNotifications();

	int counter = 0;

	for (int i = 0; i < groupsData.size(); i++)
	{
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			if (groupsData[i].students[j].bCanReceiveScholarship)
				counter++;
		}
	}

	std::cout << "Number of students receiving a scholarship: " << counter << std::endl;

	displayClsWithPause();
}

void practicalWork1::displayStudentNotReceivingScholarships() {
	displayNotifications();

	bool alredyDisplayed = false;

	for (int i = 0; i < groupsData.size(); i++)
	{
		alredyDisplayed = false;
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			if (!groupsData[i].students[j].bCanReceiveScholarship) {
				if (!alredyDisplayed) {
					alredyDisplayed = true;
					displayGroupInfo(i);
					std::cout << "Students:" << std::endl;
				}
				displayStudent(groupsData[i].students[j]);
			}
		}
	}

	displayClsWithPause();
}

void practicalWork1::displayStudentsByGrades(int minGrade) {
	displayNotifications();

	bool alredyDisplayed = false,
		canBeDisplayed = false;

	for (int i = 0; i < groupsData.size(); i++)
	{
		alredyDisplayed = false;
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			canBeDisplayed = true;
			for (int k = 0; k < 3; k++)
				if (groupsData[i].students[j].lastSessionGrades.exam[k] < minGrade)
					canBeDisplayed = false;
			for (int k = 0; k < 5; k++)
				if (groupsData[i].students[j].lastSessionGrades.diffTest[k] < minGrade)
					canBeDisplayed = false;

			if (canBeDisplayed) {
				if (!alredyDisplayed) {
					alredyDisplayed = true;
					displayGroupInfo(i);
					std::cout << "Students:" << std::endl;
				}
				displayStudent(groupsData[i].students[j]);
			}
		}
	}


	displayClsWithPause();
}

void practicalWork1::displayStudentsBySequentialNumberInGroupList(int sequentialNumberInGroupList) {
	displayNotifications();
	bool alredyDisplayed = false;

	for (int i = 0; i < groupsData.size(); i++)
	{
		alredyDisplayed = false;
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			if (groupsData[i].students[j].sequentialNumberInGroupList == sequentialNumberInGroupList) {
				if (!alredyDisplayed) {
					alredyDisplayed = true;
					displayGroupInfo(i);
					std::cout << "Students:" << std::endl;
				}
				displayStudent(groupsData[i].students[j]);
			}
		}
	}

	displayClsWithPause();
}

void practicalWork1::displayByTimeOfLastChange(time_t minTime, time_t maxTime) {
	displayNotifications();

	bool alredyDisplayed = false;

	for (int i = 0; i < groupsData.size(); i++)
	{
		alredyDisplayed = false;
		for (int j = 0; j < groupsData[i].students.size(); j++)
		{
			if (groupsData[i].students[j].timeOfLastChange >= minTime && groupsData[i].students[j].timeOfLastChange <= maxTime) {
				if (!alredyDisplayed) {
					alredyDisplayed = true;
					displayGroupInfo(i);
					std::cout << "Students:" << std::endl;
				}
				displayStudent(groupsData[i].students[j]);
			}
		}
	}

	displayClsWithPause();
}