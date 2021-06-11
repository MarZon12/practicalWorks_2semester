//In reality, it was necessary to describe a separate class for the student and the group, but I'm too lazy for that.
//Just packed all the practical work into one class.
//@MarZon

#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class practicalWork1
{
public:
	practicalWork1();
	~practicalWork1();
	
	

	//Menus
	void menu_main();
	void menu_groupSelect();
	void menu_addNewStudent();
	void menu_editStudentData();
	void menu_daySelect();

private:
	enum class formOfEducation
	{
		FULLTIME,
		PARTTIME,
		EXTRAMURAL
	};
	struct sessionGrades {
		unsigned short exam[3];
		unsigned short diffTest[5];

		void operator=(sessionGrades sessionGrades_) {
			for (int i = 0; i < 3; i++)
				this->exam[i] = sessionGrades_.exam[i];
			for (int i = 0; i < 5; i++)
				this->diffTest[i] = sessionGrades_.diffTest[i];
		}

		bool operator==(sessionGrades sessionGrades_) {
			for (int i = 0; i < 3; i++)
				if (this->exam[i] != sessionGrades_.exam[i])
					return false;
			for (int i = 0; i < 5; i++)
				if (this->diffTest[i] != sessionGrades_.diffTest[i])
					return false;
			return true;
		}
		sessionGrades() :exam{ 0, 0, 0 }, diffTest{ 0, 0, 0, 0, 0 } {};
	};
	struct student {
		std::string fullName;
		bool bIsWoman;
		short int sequentialNumberInGroupList;
		sessionGrades lastSessionGrades;
		bool bCanReceiveScholarship;
		time_t timeOfLastChange;

		student(std::string fullName_, bool bIsWoman_, short int sequentialNumberInGroupList_, sessionGrades lastSessionGrades_) :
			fullName(fullName_), bIsWoman(bIsWoman_), sequentialNumberInGroupList(sequentialNumberInGroupList_), lastSessionGrades(lastSessionGrades_), bCanReceiveScholarship(false), timeOfLastChange(0) {};

		student() :fullName("Unknown Student"), bIsWoman(false), sequentialNumberInGroupList(0), lastSessionGrades(sessionGrades()), bCanReceiveScholarship(false), timeOfLastChange(0) {};


		bool operator==(student student_) {
			if (this->fullName != student_.fullName)
				return false;

			if (this->bIsWoman != student_.bIsWoman)
				return false;

			if (this->sequentialNumberInGroupList != student_.sequentialNumberInGroupList)
				return false;

			//lastSessionGrades
			if (!(this->lastSessionGrades == student_.lastSessionGrades))
				return false;

			if (this->bCanReceiveScholarship != student_.bCanReceiveScholarship)
				return false;

			//if (this->timeOfLastChange != student_.timeOfLastChange)
			//	return false;

			return true;
		}

		void operator=(student student_) {
			this->fullName = student_.fullName;
			this->bIsWoman = student_.bIsWoman;
			this->sequentialNumberInGroupList = student_.sequentialNumberInGroupList;
			this->lastSessionGrades = student_.lastSessionGrades;
			this->bCanReceiveScholarship = student_.bCanReceiveScholarship;
			this->timeOfLastChange = student_.timeOfLastChange;
		}

	};
	struct groupData
	{
		int groupNumber;
		formOfEducation groupFormOfEducation;
		std::vector<student>students;
		groupData() :groupNumber(0), groupFormOfEducation(formOfEducation::FULLTIME) {};
		groupData(int groupNumber_, formOfEducation formOfEducation_) :groupNumber(groupNumber_), groupFormOfEducation(formOfEducation_) {};
	};
	
	//LoadData
	enum class LD_dataTypeInString
	{
		UNKNOWN,
		SAVEVERSION,
		NEW_GROUP,
		NEW_STUDENT,
		GROUP_NUMBER,
		GROUP_FORMOFEDUCATIONCODE,
		STUDENT_FULLNAME,
		STUDENT_SEXCODE,
		STUDENT_SEQUENTIALNUMBERIBGROUPLIST,
		STUDENT_LASTSESSIONGRADES,
		STUDENT_TIMEOFLASTCHANGE
	};
	struct LD_DTIS_withPos
	{
		LD_dataTypeInString dataType;
		size_t pos;
		LD_DTIS_withPos() :dataType(LD_dataTypeInString::UNKNOWN), pos(-1){};
		LD_DTIS_withPos(LD_dataTypeInString dataType_, size_t pos_) :dataType(dataType_), pos(pos_) {};
	};
	struct LD_getNextIntFromStringResult {
		int intValue;
		bool success;
		int nextCharAfterIntPos;
		LD_getNextIntFromStringResult() :intValue(0), success(false), nextCharAfterIntPos(-1) {};
		LD_getNextIntFromStringResult(int intValue_, bool success_, int nextCharAfterIntPos_) :intValue(intValue_), success(success_), nextCharAfterIntPos(nextCharAfterIntPos_) {};
	};

	//save info
	struct saveMarkers {
		const std::string saveName;

		//mb in the future (never comes) (to enable legacy support in the save loader)
		const int version; 
		const std::string saveVersion;

		const std::string groupData;
		const std::string GROUP_Number;
		const std::string GROUP_formOfEducationCode;

		const std::string studentData;
		const std::string STUDENT_fullName;
		const std::string STUDENT_sexCode;
		const std::string STUDENT_sequentialNumberInGroupList;
		const std::string STUDENT_lastSessionGrades;
		const std::string STUDENT_timeOfLastChange;

		saveMarkers() :saveName("students.txt"), saveVersion("Save version: "), version(100),
			groupData("[GroupData]"), GROUP_Number("Group number: "), GROUP_formOfEducationCode("Form of education (code): "),
			studentData("[StudentData]"), STUDENT_fullName("Full name: "), STUDENT_sexCode("Sex (code): "), STUDENT_sequentialNumberInGroupList("Sequential number in group list: "), STUDENT_lastSessionGrades("Last session grades: "), STUDENT_timeOfLastChange("Time of last change: ") {};
	};
	
	std::vector<groupData> groupsData;
	std::vector<std::string> notificationsToDisplay;

	//Notifications
	void addNotification(std::string notification);
	void displayNotifications();



	//Instruments
	void clearCin();

	bool canStudentReceiveScholarship(const sessionGrades& studentLastSessionGrades, const formOfEducation& studentFormOfEducation);

	std::string formOfEducationToString(formOfEducation formOfEducation_);

	void displayClsWithPause();



	//Saving and loading
	bool saveData();
	
	//pos - the first character after the word indicating the data type in the string
	LD_DTIS_withPos LD_getDataTypeInString(const std::string& stringToAnalyze);
	//pos - first character after int found in string
	LD_getNextIntFromStringResult LD_getNextIntFromString(std::string stringToAnalyze, int pos);
	bool loadData();



	bool ANS_checkStudentCanBeAddedInGroup(const groupData& possibleGroup, const student& studentData, const formOfEducation& studentFormOfEducation);
	bool addNewStudent(const student& studentToAdd, const formOfEducation& studentFormOfEducation, const int& studentGroupNumber);



	// true = need to edit
	bool ESD_processData(int intToProcess);
	// true = need to edit
	bool ESD_processData(const std::string& stringToProcess);
	// always true: correcting finalSessionGradesRef
	bool ESD_processData(const sessionGrades& currentSessionGradesRef, const sessionGrades& newSessionGradesRef, sessionGrades& finalSessionGradesRef);
	// -1 if not existing
	int ESD_getGroupIndex(int groupNumber);
	// true is success
	bool ESD_removeStudentFromGroup(groupData& groupDataRef, student& studentDataRef);

	void editStudentData(student* studentDataRef, groupData* studentGroupDataRef, student& newStudentDataRef, groupData newStudentGroupData);

	

	//reusable display functions
	void displayStudent(const student& studentRef);
	void displayGroupInfo(int groupIndex);



	void displayAllStudents();
	
	void displayAllStudentsOfGroup(int groupIndex);

	void performCombSort(const int studentsCount, int* studentstGroupIndex, int* studentsSelfIndex, int* studentsSelfSumGrade);



	int getStudentsCount();

	void displayTopStudentsOfThePreviousSession();
	
	void displayManAndWomanCount();
	
	void displayCountOfStudentReceivingScholarships();
	
	void displayStudentNotReceivingScholarships();

	void displayStudentsByGrades(int minGrade);

	void displayStudentsBySequentialNumberInGroupList(int sequentialNumberInGroupList);

	void displayByTimeOfLastChange(time_t minTime, time_t maxTime);
};
