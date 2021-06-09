// Part of the code is protected by my copyright.
// Use and distribution are not permitted.
// @marzon

// CODE NEEDED REFACTORING

#include "PolishNotation.h"



//*******************//
//      PUBLIC       //
//*******************//


//       -CONSTRUCTOR / DESTRUCTOR-       //

PolishNotation::PolishNotation() {
	// filling in the operator weights map
	operatorWeightsMap.insert(std::make_pair(')', 4));
	operatorWeightsMap.insert(std::make_pair('^', 3));
	operatorWeightsMap.insert(std::make_pair('*', 2));
	operatorWeightsMap.insert(std::make_pair('/', 2));
	operatorWeightsMap.insert(std::make_pair('+', 1));
	operatorWeightsMap.insert(std::make_pair('-', 1));
	operatorWeightsMap.insert(std::make_pair('(', 0));

	cacheEnabled = true;
	nativeNotationType = NotationType::InvalidNotation;
}

PolishNotation::PolishNotation(const std::string& notation) {
	// filling in the operator weights map
	operatorWeightsMap.insert(std::make_pair(')', 4));
	operatorWeightsMap.insert(std::make_pair('^', 3));
	operatorWeightsMap.insert(std::make_pair('*', 2));
	operatorWeightsMap.insert(std::make_pair('/', 2));
	operatorWeightsMap.insert(std::make_pair('+', 1));
	operatorWeightsMap.insert(std::make_pair('-', 1));
	operatorWeightsMap.insert(std::make_pair('(', 0));
	
	cacheEnabled = true;
	setNativeNotation(notation);
}

PolishNotation::~PolishNotation() {

}


//       -SET NOTATION-       //

void PolishNotation::setNativeNotation(const std::string& notation) {
	// filling a vector of words
	std::vector<Word> tmpNativeNotation(stringToWordVector(notation));

	// identification notation
	NotationType tmpNativeNotationType = identifyNotation(tmpNativeNotation);
	
	// checking for exceptions
	if (tmpNativeNotationType == NotationType::InvalidNotation)
	{
		
		throw std::exception("[setNativeNotation]: Invalid notation");
	}

	// handling negative values if it is standard notation
	if (tmpNativeNotationType == NotationType::Standart)
	{
		standartNotation_handlingNegativeValues(tmpNativeNotation);
	}

	// notation validation
	StateWithMessage notationCorrectness(validateNotation(tmpNativeNotation, tmpNativeNotationType));

	// checking for exceptions
	if (notationCorrectness.state == false)
	{
		throw std::exception(notationCorrectness.message.c_str());
	}

	// initialization
	nativeNotation = tmpNativeNotation;
	nativeNotationType = tmpNativeNotationType;

	// clear cache
	cache_standartNotation.clear();
	cache_polishNotation.clear();
	cache_reversePolishNotation.clear();

	variableMap.clear();
}


//       -GET NOTATION-       //

std::vector<PolishNotation::Word> PolishNotation::getNativeNotation() const {
	return nativeNotation;
}

PolishNotation::NotationType PolishNotation::getNativeNotationType() const {
	return nativeNotationType;
}

std::string PolishNotation::getNativeNotationTypeStr() const {
	switch (nativeNotationType)
	{
	case NotationType::Standart:
		return std::string("Standart");
		break;

	case NotationType::PolishNotation:
		return std::string("Polish");
		break;

	case NotationType::ReversePolishNotation:
		return std::string("Reverse polish");
		break;

	default:
		return std::string("Invalid");
		break;
	}
}


std::vector<PolishNotation::Word> PolishNotation::getStandartNotation() const {
	if (cache_standartNotation.empty())
	{
		switch (nativeNotationType)
		{
		case NotationType::Standart:
			return nativeNotation;
			break;

		case NotationType::PolishNotation:
			if (cacheEnabled)
			{
				cache_standartNotation = polishToStandart(nativeNotation);
				return cache_standartNotation;
			}
			return polishToStandart(nativeNotation);
			break;

		case NotationType::ReversePolishNotation:
			if (cacheEnabled)
			{
				cache_standartNotation = reversePolishToStandart(nativeNotation);
				return cache_standartNotation;
			}
			return reversePolishToStandart(nativeNotation);
			break;

		default:
			throw std::exception("[getStandartNotation]: Unable to get standart notation: unknown type of native notation");
			break;
		}
	}
	else
	{
		return cache_standartNotation;
	}
}

std::vector<PolishNotation::Word> PolishNotation::getPolishNotation() const {
	if (cache_polishNotation.empty())
	{
		switch (nativeNotationType)
		{
		case NotationType::Standart:
			if (cacheEnabled)
			{
				cache_polishNotation = standartToPolish(nativeNotation);
				return cache_polishNotation;
			}
			return standartToPolish(nativeNotation);
			break;

		case NotationType::PolishNotation:
			return nativeNotation;
			break;

		case NotationType::ReversePolishNotation:
			if (cacheEnabled)
			{
				cache_polishNotation = reversePolishToPolish(nativeNotation);
				return cache_polishNotation;
			}
			return reversePolishToPolish(nativeNotation);
			break;

		default:
			throw std::exception("[getPolishNotation]: Unable to get polish notation: unknown type of native notation");
			break;
		}
	}
	else
	{
		return cache_polishNotation;
	}
}

std::vector<PolishNotation::Word> PolishNotation::getReversePolishNotation() const {
	if (cache_reversePolishNotation.empty())
	{
		switch (nativeNotationType)
		{
		case NotationType::Standart:
			if (cacheEnabled)
			{
				cache_reversePolishNotation = standartToReversePolish(nativeNotation);
				return cache_reversePolishNotation;
			}
			return standartToReversePolish(nativeNotation);
			break;

		case NotationType::PolishNotation:
			if (cacheEnabled)
			{
				cache_reversePolishNotation = polishToReversePolish(nativeNotation);
				return cache_reversePolishNotation;
			}
			return polishToReversePolish(nativeNotation);
			break;

		case NotationType::ReversePolishNotation:
			return nativeNotation;
			break;

		default:
			throw std::exception("[getReversePolishNotation]: Unable to get reverse polish notation: unknown type of native notation");
			break;
		}
	}
	else
	{
		return cache_reversePolishNotation;
	}
	
}


std::string PolishNotation::notationToString(std::vector<Word> notation) {
	if (notation.empty())
	{
		throw std::exception("[notationToString]: Notation is empty");
	}

	std::string strNotation;

	for (size_t i = 0; i < notation.size(); i++)
	{
		strNotation += notation[i].data;
		strNotation += ' ';
	}

	return strNotation;
};


//       -GET STATUS-       //

bool PolishNotation::empty() const {
	if (nativeNotation.size() == 0)
	{
		return true;
	}

	return false;
}

size_t PolishNotation::size() const {
	return nativeNotation.size();
}

PolishNotation::Word PolishNotation::operator[](size_t i) const {
	if (i >= nativeNotation.size())
	{
		throw std::exception("[PolishNotation]: bad index");
	}

	return nativeNotation[i];
}


bool PolishNotation::getCacheState() {
	return cacheEnabled;
}

void PolishNotation::setCacheState(bool enabled) {
	cacheEnabled = enabled;

	// clear cache
	if (!enabled)
	{
		cache_standartNotation.clear();
		cache_polishNotation.clear();
		cache_reversePolishNotation.clear();
	}
}

//       -VARIABLES-       //

PolishNotation::StateWithMessage PolishNotation::getNameFirstUnsetVariable() const {
	long long unsetVariableIndex = 0;

	std::map<std::string, long double>::iterator variableMapIterator;

	while (true)
	{
		unsetVariableIndex = findWordIndexByType(nativeNotation, WordType::Variable, true, unsetVariableIndex + 1);
		if (unsetVariableIndex >= 0)
		{
			variableMapIterator = variableMap.find(nativeNotation[unsetVariableIndex].data);

			if (variableMapIterator != variableMap.end()) // if found in variableMap == already set
			{
				continue;
			}
			else
			{
				return StateWithMessage(nativeNotation[unsetVariableIndex].data, true);
			}
		}
		else
		{
			return StateWithMessage("Unset variable not found", false);
		}
	}
}

PolishNotation::StateWithMessage PolishNotation::setFirstUnsetVariable(const long double value) const {
	long long unsetVariableIndex = 0;

	std::map<std::string, long double>::iterator variableMapIterator;

	while (true)
	{
		unsetVariableIndex = findWordIndexByType(nativeNotation, WordType::Variable, true, unsetVariableIndex + 1);
		if (unsetVariableIndex >= 0)
		{
			variableMapIterator = variableMap.find(nativeNotation[unsetVariableIndex].data);

			if (variableMapIterator != variableMap.end()) // if found in variableMap == already set
			{
				continue;
			}
			else
			{
				variableMap.insert(std::make_pair(nativeNotation[unsetVariableIndex].data, value));
				return StateWithMessage("Unset variable set", true);
			}
		}
		else
		{
			return StateWithMessage("Unset variable not found", false);
		}
	}
}


PolishNotation::StateWithMessage PolishNotation::getNameNextSetVariable() const {
	if (variableMap.empty())
	{
		return StateWithMessage("No variables set", false);
	}

	if (variableMapIterator == variableMap.end())
	{
		variableMapIterator = variableMap.begin();
	}
	else
	{
		variableMapIterator++;
	}

	if (variableMapIterator == variableMap.end())
	{
		variableMapIterator = variableMap.begin();
		return StateWithMessage("These are all variables. The position is shifted to the beginning", false);
	}
	else
	{
		return StateWithMessage(variableMapIterator->first, true);
	}
}

PolishNotation::StateWithMessage PolishNotation::setNextSetVariable(const long double value) const {
	if (variableMap.empty())
	{
		return StateWithMessage("No variables set", false);
	}

	if (variableMapIterator == variableMap.end())
	{
		return StateWithMessage("Variable not found", false);
	}
	else
	{
		variableMapIterator->second = value;
		return StateWithMessage(std::string("The variable ") + variableMapIterator->first + " is set", true);
	}
}


std::map<std::string, long double> PolishNotation::getVariableMap() const {
	return variableMap;
}

std::map<std::string, long double>& PolishNotation::getVariableMapRef() const {
	return variableMap;
}


//       -CALCULATIONS-       //

long double PolishNotation::getCalculatedNotation() const {
	std::vector<Word> notation(getReversePolishNotation());

	std::vector<MathWord> mathNotation(wordToMathWord(notation));

	long long mathOperatorIndex = 0;

	long double intermediateValue = 0;

	while (mathNotation.size() != 1) {
		mathOperatorIndex = findWordIndexByType(mathNotation, WordType::MathOperator, true);

		if (mathOperatorIndex == -1)
		{
			break;
		}

		if (mathOperatorIndex - 2 >= 0)
		{
			switch (mathNotation[mathOperatorIndex].mathOperator[0])
			{
			case '-':
				intermediateValue += mathNotation[mathOperatorIndex - 2].value - mathNotation[mathOperatorIndex - 1].value;
				break;

			case '+':
				intermediateValue += mathNotation[mathOperatorIndex - 2].value + mathNotation[mathOperatorIndex - 1].value;
				break;

			case '/':
				intermediateValue += mathNotation[mathOperatorIndex - 2].value / mathNotation[mathOperatorIndex - 1].value;
				break;

			case '*':
				intermediateValue += mathNotation[mathOperatorIndex - 2].value * mathNotation[mathOperatorIndex - 1].value;
				break;

			case '^':
				intermediateValue += pow(mathNotation[mathOperatorIndex - 2].value, mathNotation[mathOperatorIndex - 1].value);
				break;

			default:
				throw std::exception("[getCalculatedNotation]: Unknown mathematical operator");
				break;
			}

			mathNotation[mathOperatorIndex - 2].value = intermediateValue;

			intermediateValue = 0;
			mathNotation.erase(mathNotation.begin() + mathOperatorIndex);
			mathNotation.erase(mathNotation.begin() + mathOperatorIndex - 1);
		}
		else
		{
			throw std::exception("[getCalculatedNotation]: An unexpected error occurred");
		}
	}

	return mathNotation[0].value;
}



//********************//
//      PRIVATE       //
//********************//


//       -BASIC REUSABLE FUNCTIONS-       //

long long PolishNotation::findWordIndexByType(const std::vector<Word>& notation, const WordType& type, bool findFirst, const long long limIndex) const {
	size_t startIndex;

	if (findFirst)
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = 0;
		}

		for (size_t i = startIndex; i < notation.size(); i++)
		{
			if (notation[i].type == type)
			{
				return i;
			}
		}
	}
	else
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = notation.size() - 1;
		}

		for (long long i = startIndex; i >= 0; i--)
		{
			if (notation[i].type == type)
			{
				return i;
			}
		}
	}

	return -1;
}

long long PolishNotation::findWordIndexByType(const std::vector<MathWord>& notation, const WordType& type, bool findFirst, const long long limIndex) const {
	size_t startIndex;

	if (findFirst)
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = 0;
		}

		for (size_t i = startIndex; i < notation.size(); i++)
		{
			if (notation[i].type == type)
			{
				return i;
			}
		}
	}
	else
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = notation.size() - 1;
		}

		for (long long i = startIndex; i >= 0; i--)
		{
			if (notation[i].type == type)
			{
				return i;
			}
		}
	}

	return -1;
}

long long PolishNotation::findWordIndexByData(const std::vector<Word>& notation, const std::string& data, const bool findFirst, const long long limIndex) const {
	size_t startIndex;

	if (findFirst)
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = 0;
		}

		for (size_t i = startIndex; i < notation.size(); i++)
		{
			if (notation[i].data == data)
			{
				return i;
			}
		}
	}
	else
	{
		if (limIndex >= 0)
		{
			startIndex = limIndex;
		}
		else
		{
			startIndex = notation.size() - 1;
		}

		for (long long i = startIndex; i >= 0; i--)
		{
			if (notation[i].data == data)
			{
				return i;
			}
		}
	}

	return -1;
}


std::vector<PolishNotation::Word> PolishNotation::stringToWordVector(const std::string& notation) const {
	std::vector<Word> wordVector;

	std::string currentWord;

	WordType currentWordType = WordType::NotIdentified;

	// checking for exceptions
	std::string errorString;

	unsigned int countOfOpenRoundBracket = 0;
	unsigned int countOfСlosingRoundBracket = 0;

	for (size_t i = 0; i < notation.size(); i++)
	{
		if (notation[i] == ' ')
		{
			continue;
		}

		if (notation[i] == '.' || notation[i] == ',')
		{
			if ((i + 1 < notation.size()) && (i > 0))
			{
				if ((notation[i - 1] >= '0' && notation[i - 1] <= '9') && (notation[i + 1] >= '0' && notation[i + 1] <= '9'))
				{
					continue;
				}
				else
				{
					throw std::exception("The symbol '.' or ',' occurs, although it is not part of the number");
				}
			}
			else
			{
				throw std::exception("The symbol '.' or ',' occurs, although it is not part of the number");
			}
		}

		if (notation[i] == '(')
		{
			countOfOpenRoundBracket++;
			continue;
		}

		if (notation[i] == ')')
		{
			countOfСlosingRoundBracket++;
			continue;
		}

		if (notation[i] >= '0' && notation[i] <= '9')
		{
			continue;
		}

		if (notation[i] >= 'A' && notation[i] <= 'Z' || notation[i] >= 'a' && notation[i] <= 'z')
		{
			continue;
		}

		if (notation[i] == '+' || notation[i] == '-' || notation[i] == '*' || notation[i] == '/' || notation[i] == '^')
		{
			continue;
		}

		errorString = "Unknown invalid character detected (";
		errorString += notation[i];
		errorString += ')';
		throw std::exception(errorString.c_str());
	}

	if (countOfOpenRoundBracket != countOfСlosingRoundBracket)
	{
		throw std::exception("[stringToWordVector]: Round brackets do not open/close correctly");
	}



	// filling a vector of words
	for (size_t i = 0; i < notation.size(); i++)
	{
		if (notation[i] == ' ')
		{
			if (!currentWord.empty())
			{
				wordVector.push_back(Word(currentWord, currentWordType));
				currentWord.clear();
				continue;
			}
			continue;
		}


		if (notation[i] == '.' || notation[i] == ',')
		{
			currentWord.push_back('.');
			continue;
		}


		if (notation[i] == '(' || notation[i] == ')')
		{
			if (!currentWord.empty())
			{
				wordVector.push_back(Word(currentWord, currentWordType));
				currentWord.clear();
			}

			currentWord.push_back(notation[i]);
			currentWordType = WordType::RoundBracket;

			continue;
		}


		if (notation[i] >= '0' && notation[i] <= '9')
		{
			if (currentWordType != WordType::Number && !currentWord.empty())
			{
				wordVector.push_back(Word(currentWord, currentWordType));
				currentWord.clear();
			}

			currentWord.push_back(notation[i]);
			currentWordType = WordType::Number;

			continue;
		}


		if (notation[i] >= 'A' && notation[i] <= 'Z' || notation[i] >= 'a' && notation[i] <= 'z')
		{
			if (currentWordType != WordType::Variable && !currentWord.empty())
			{
				wordVector.push_back(Word(currentWord, currentWordType));
				currentWord.clear();
			}

			currentWord.push_back(notation[i]);
			currentWordType = WordType::Variable;

			continue;
		}


		if (notation[i] == '+' || notation[i] == '-' || notation[i] == '*' || notation[i] == '/' || notation[i] == '^')
		{
			if (!currentWord.empty())
			{
				wordVector.push_back(Word(currentWord, currentWordType));
				currentWord.clear();
			}

			currentWord.push_back(notation[i]);
			currentWordType = WordType::MathOperator;
		}
	}
	// enter the last word (relevant if the input string does not end with a space)
	if (!currentWord.empty())
	{
		wordVector.push_back(Word(currentWord, currentWordType));
	}

	return wordVector;
}

void PolishNotation::reverse(std::vector<Word>& notation) const {
	std::vector<Word> notationNew;
	for (long long i = notation.size() - 1; i >= 0; i--)
	{
		notationNew.push_back(notation[i]);
	}

	for (size_t i = 0; i < notationNew.size(); i++)
	{
		if (notationNew[i].data == "(")
		{
			notationNew[i].data = ")";
			continue;
		}
		if (notationNew[i].data == ")")
		{
			notationNew[i].data = "(";
			continue;
		}
	}

	notation = notationNew;
}

// The code below is protected by my copyright.
// Use and distribution are not permitted.
void PolishNotation::clearExtraBrackets(std::vector<Word>& notation) const {
	std::vector<Word> tmpNotation(notation);

	long long openBracketIndex = 0;
	long long closeBracketIndex = 0;

	short leftOperatorPriority = 0;
	short rightOperatorPriority = 0;
	short innerOperatorPriority = 0;

	long long leftOperatorIndex = -1;
	long long rightOperatorIndex = -1;
	long long innerOperatorIndex = -1;

	std::string storage;

	while (true)
	{
		openBracketIndex = findWordIndexByData(tmpNotation, "(", false);
		if (openBracketIndex == -1)
		{
			break;
		}

		closeBracketIndex = findWordIndexByData(tmpNotation, ")", true, openBracketIndex);
		if (closeBracketIndex == -1)
		{
			break;
		}



		// get leftOperatorPriority
		leftOperatorPriority = 0;
		leftOperatorIndex = -1;
		if (openBracketIndex - 1 >= 0) // if valid
		{
			if (tmpNotation[openBracketIndex - 1].type == WordType::MathOperator)
			{
				leftOperatorPriority = operatorWeightsMap.at(tmpNotation[openBracketIndex - 1].data[0]);
				leftOperatorIndex = openBracketIndex - 1;
			}
		}

		// get rightOperatorPriority
		rightOperatorPriority = 0;
		rightOperatorIndex = -1;
		if (closeBracketIndex + 1 < tmpNotation.size()) // if valid
		{
			if (tmpNotation[closeBracketIndex + 1].type == WordType::MathOperator)
			{
				rightOperatorPriority = operatorWeightsMap.at(tmpNotation[closeBracketIndex + 1].data[0]);
				rightOperatorIndex = closeBracketIndex + 1;
			}
		}

		// get innerOperatorPriority
		// can be optimized (using index)
		innerOperatorPriority = 0;
		innerOperatorIndex = -1;
		for (size_t i = openBracketIndex + 1; i < closeBracketIndex; i++)
		{
			if (tmpNotation[i].type == WordType::MathOperator)
			{
				innerOperatorPriority = operatorWeightsMap.at(tmpNotation[i].data[0]);
				innerOperatorIndex = i;
				break;
			}
		}

		if (innerOperatorIndex == -1)
		{
			throw std::exception("[clearExtraBrackets]: Critical error: inner operator not found"); //WARNING
		}



		if (leftOperatorIndex >= 0)
		{
			if (rightOperatorIndex >= 0)
			{
				if (tmpNotation[leftOperatorIndex].data == tmpNotation[innerOperatorIndex].data && (tmpNotation[leftOperatorIndex].data == "+" || tmpNotation[leftOperatorIndex].data == "*"))
				{
					if (rightOperatorPriority <= innerOperatorPriority ||
						(tmpNotation[rightOperatorIndex].data == tmpNotation[innerOperatorIndex].data && (tmpNotation[rightOperatorIndex].data == "+" || tmpNotation[rightOperatorIndex].data == "*")))
					{
						//delete bracket
						tmpNotation.erase(tmpNotation.begin() + closeBracketIndex);
						tmpNotation.erase(tmpNotation.begin() + openBracketIndex);
						closeBracketIndex = closeBracketIndex - 2;
					}
				}
				else
				{
					if (leftOperatorPriority < innerOperatorPriority ||
						(tmpNotation[rightOperatorIndex].data == tmpNotation[innerOperatorIndex].data && (tmpNotation[rightOperatorIndex].data == "+" || tmpNotation[rightOperatorIndex].data == "*")))
					{
						//delete bracket
						tmpNotation.erase(tmpNotation.begin() + closeBracketIndex);
						tmpNotation.erase(tmpNotation.begin() + openBracketIndex);
						closeBracketIndex = closeBracketIndex - 2;
					}
				}
			}
			else
			{
				if (leftOperatorPriority < innerOperatorPriority ||
					tmpNotation[leftOperatorIndex].data == tmpNotation[innerOperatorIndex].data && (tmpNotation[leftOperatorIndex].data == "+" || tmpNotation[leftOperatorIndex].data == "*"))
				{
					//delete bracket
					tmpNotation.erase(tmpNotation.begin() + closeBracketIndex);
					tmpNotation.erase(tmpNotation.begin() + openBracketIndex);
					closeBracketIndex = closeBracketIndex - 2;
				}
			}
		}
		else
		{
			if (rightOperatorIndex >= 0)
			{
				if (rightOperatorPriority <= innerOperatorPriority ||
					(tmpNotation[rightOperatorIndex].data == tmpNotation[innerOperatorIndex].data && (tmpNotation[rightOperatorIndex].data == "+" || tmpNotation[rightOperatorIndex].data == "*")))
				{
					//delete bracket
					tmpNotation.erase(tmpNotation.begin() + closeBracketIndex);
					tmpNotation.erase(tmpNotation.begin() + openBracketIndex);
					closeBracketIndex = closeBracketIndex - 2;
				}
			}
			else
			{
				//delete bracket
				tmpNotation.erase(tmpNotation.begin() + closeBracketIndex);
				tmpNotation.erase(tmpNotation.begin() + openBracketIndex);
				closeBracketIndex = closeBracketIndex - 2;
			}
		}

		// copy the data inside the brackets
		for (size_t i = openBracketIndex; i <= closeBracketIndex; i++)
		{
			storage += tmpNotation[i].data;
		}

		tmpNotation[openBracketIndex].data = storage;
		tmpNotation[openBracketIndex].type = WordType::Variable;

		storage.clear();
		for (long long i = closeBracketIndex; i > openBracketIndex; i--)
		{
			tmpNotation.erase(tmpNotation.begin() + i);
		}
	}

	notation = stringToWordVector(tmpNotation[0].data);
}

void PolishNotation::standartNotation_handlingNegativeValues(std::vector<Word>& notation) {
	if (notation.empty())
	{
		return;
	}
	
	std::vector<Word> tmpNotation;

	for (long long i = 0; i < notation.size(); i++)
	{
		if (notation[i].type == WordType::MathOperator)
		{
			if (notation[i].data == "-")
			{
				if (i - 1 >= 0)
				{
					if (notation[i - 1].type == WordType::RoundBracket || notation[i - 1].type == WordType::MathOperator)
					{
						if (notation[i - 1].type == WordType::MathOperator && i + 1 < notation.size())
						{
							if (notation[i + 1].type == WordType::Number || notation[i + 1].type == WordType::Variable)
							{
								tmpNotation.push_back(Word("(", WordType::RoundBracket));
								tmpNotation.push_back(Word("0", WordType::Number));
								tmpNotation.push_back(notation[i]);
								tmpNotation.push_back(notation[i + 1]);
								tmpNotation.push_back(Word(")", WordType::RoundBracket));
								i++;
								continue;
							}
						}

						if (notation[i - 1].data == "(")
						{
							tmpNotation.push_back(Word("0", WordType::Number));
							tmpNotation.push_back(notation[i]);
						}
						else
						{
							tmpNotation.push_back(notation[i]);
						}
					}
					else
					{
						tmpNotation.push_back(notation[i]);
					}
				}
				else
				{
					if (i + 1 < notation.size())
					{
						if (notation[i + 1].type == WordType::Number || notation[i + 1].type == WordType::Variable)
						{
							tmpNotation.push_back(Word("(", WordType::RoundBracket));
							tmpNotation.push_back(Word("0", WordType::Number));
							tmpNotation.push_back(notation[i]);
							tmpNotation.push_back(notation[i + 1]);
							tmpNotation.push_back(Word(")", WordType::RoundBracket));
							i++;
						}
						else
						{
							tmpNotation.push_back(notation[i]);
						}
					}
					else
					{
						tmpNotation.push_back(notation[i]);
					}
				}
			}
			else
			{
				tmpNotation.push_back(notation[i]);
			}
		}
		else
		{
			tmpNotation.push_back(notation[i]);
		}
	}

	notation = tmpNotation;
}


//       -VALIDATORS-       //

PolishNotation::NotationType PolishNotation::identifyNotation(const std::vector<Word>& notation) const {
	bool foundTwoVariablesInRow = false;

	for (size_t i = 0; i < notation.size(); i++)
	{
		if (notation[i].type == WordType::RoundBracket)
		{
			return NotationType::Standart;
		}

		if (i > 0)
		{
			if ((notation[i - 1].type == WordType::Number || notation[i - 1].type == WordType::Variable) && notation[i].type == notation[i - 1].type)
			{
				foundTwoVariablesInRow = true;

				if (notation[notation.size() - 1].type == WordType::MathOperator)
				{
					return NotationType::ReversePolishNotation;
				}

				if (notation[0].type == WordType::MathOperator)
				{
					return NotationType::PolishNotation;
				}
			}
		}
	}

	if (!foundTwoVariablesInRow)
	{
		return NotationType::Standart;
	}
	else
	{
		return NotationType::InvalidNotation;
	}
}


PolishNotation::StateWithMessage PolishNotation::validateNotation(const std::vector<Word>& notation, const NotationType& notationType) const {
	if (notationType == NotationType::Standart)
	{
		return validateNotation_Standart(notation);
	}

	if (notationType == NotationType::PolishNotation)
	{
		return validateNotation_Polish(notation);
	}

	if (notationType == NotationType::ReversePolishNotation)
	{
		return validateNotation_ReversePolish(notation);
	}

	return StateWithMessage();
}

PolishNotation::StateWithMessage PolishNotation::validateNotation_Standart(const std::vector<Word>& notation) const {
	if (notation.size() == 0)
	{
		return StateWithMessage("Notation is empty", false);
	}
	
	unsigned int countOfOpenRoundBracket = 0;
	unsigned int countOfСlosingRoundBracket = 0;

	for (size_t i = 0; i < notation.size(); i++)
	{
		// RoundBracket check
		if (notation[i].type == WordType::RoundBracket)
		{
			// extra code, this check goes in the "PolishNotation::setNativeNotation" function
			if (notation[i].data == "(")
			{
				countOfOpenRoundBracket++;
			}
			else
			{
				countOfСlosingRoundBracket++;
			}

			// checks the situation: 2 ( + 3) (correct: 2 + 3)
			if (i + 1 < notation.size() && notation[i].data == "(")
			{
				if (notation[i + 1].type == WordType::MathOperator)
				{
					return StateWithMessage("Round brackets do not open correctly", false);
				}
			}

			// checks the situation: (2 + ) 3 (correct: 2 + 3)
			if (i > 0 && notation[i].data == ")")
			{
				if (notation[i - 1].type == WordType::MathOperator)
				{
					return StateWithMessage("Round brackets do not close correctly", false);
				}
			}
		}
		
		// MathOperator check
		if (notation[i].type == WordType::MathOperator)
		{
			if (i + 1 < notation.size())
			{
				// checks the situation: 3 - - 4 (correct: 3 - 4)
				if (notation[i + 1].type == WordType::MathOperator)
				{
					return StateWithMessage("Double math operator is not allowed", false);
				}
			}

			if (i == 0 || i == notation.size() - 1)
			{
				return StateWithMessage("Math operator is incorrectly positioned", false);
			}
		}

		if (notation[i].type == WordType::Number || notation[i].type == WordType::Variable)
		{
			if (i + 1 < notation.size())
			{
				// checks the situation: 1 3 + 4 (correct: 3 + 4 or 1 + 4)
				if (notation[i + 1].type == WordType::Number || notation[i + 1].type == WordType::Variable)
				{
					return StateWithMessage("Double number or variable is not allowed", false);
				}
			}
		}
	}

	// RoundBracket check
	// extra code, this check goes in the "PolishNotation::setNativeNotation" function
	if (countOfOpenRoundBracket != countOfСlosingRoundBracket)
	{
		return StateWithMessage("Round brackets do not open/close correctly", false);
	}


	return StateWithMessage("Notation is correct", true);
}

PolishNotation::StateWithMessage PolishNotation::validateNotation_Polish(const std::vector<Word>& notation) const {
	if (notation.size() == 0)
	{
		return StateWithMessage("Notation is empty", false);
	}

	long long mathOperatorIndex;

	std::vector<Word> tmpNotation(notation);

	do 
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, false);

		if (mathOperatorIndex < 0)
		{
			break;
		}

		if (mathOperatorIndex + 2 < tmpNotation.size())
		{
			if (!(tmpNotation[mathOperatorIndex + 1].type == WordType::Number || tmpNotation[mathOperatorIndex + 1].type == WordType::Variable))
			{
				return StateWithMessage("Number or variable is incorrectly positioned", false);
			}

			if (tmpNotation[mathOperatorIndex + 2].type == WordType::Number || tmpNotation[mathOperatorIndex + 2].type == WordType::Variable)
			{
				tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex + 2);
				tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex);
			}
			else
			{
				return StateWithMessage("Math operator is incorrectly positioned", false);
			}
		}
		else
		{
			if (mathOperatorIndex != 0)
			{
				return StateWithMessage("Math operator is incorrectly positioned", false);
			}
			else
			{
				return StateWithMessage("Extra math operator found", false);
			}
		}
	} while (mathOperatorIndex != -1);

	if (tmpNotation.size() != 1)
	{
		return StateWithMessage("Extra number or variable found", false);
	}

	return StateWithMessage("Notation is correct", true);
}

PolishNotation::StateWithMessage PolishNotation::validateNotation_ReversePolish(const std::vector<Word>& notation) const {
	if (notation.size() == 0)
	{
		return StateWithMessage("Notation is empty", false);
	}

	long long mathOperatorIndex;

	std::vector<Word> tmpNotation(notation);

	do
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, true);

		if (mathOperatorIndex < 0)
		{
			break;
		}

		if (mathOperatorIndex - 2 >= 0)
		{
			if (!(tmpNotation[mathOperatorIndex - 1].type == WordType::Number || tmpNotation[mathOperatorIndex - 1].type == WordType::Variable))
			{
				return StateWithMessage("Number or variable is incorrectly positioned", false);
			}

			if (tmpNotation[mathOperatorIndex - 2].type == WordType::Number || tmpNotation[mathOperatorIndex - 2].type == WordType::Variable)
			{
				tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex);
				tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex - 1);
			}
			else
			{
				return StateWithMessage("Math operator is incorrectly positioned", false);
			}
		}
		else
		{
			if (mathOperatorIndex < tmpNotation.size())
			{
				return StateWithMessage("Math operator is incorrectly positioned", false);
			}
			else
			{
				return StateWithMessage("Extra math operator found", false);
			}
		}

	} while (mathOperatorIndex != -1);
	
	if (tmpNotation.size() != 1)
	{
		return StateWithMessage("Extra number or variable found", false);
	}

	return StateWithMessage("Notation is correct", true);
}


//       -CONVERTERS-       //

// The code below is protected by my copyright.
// Use and distribution are not permitted.
std::vector<PolishNotation::Word> PolishNotation::standartToPolish(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[standartToPolish]: empty notation in arguments");
	}
	
	std::vector<Word> reversedNotation(notation);

	reverse(reversedNotation);

	std::vector<Word> polishNotation;
	std::stack<Word> mathOperatorStack;

	for (size_t i = 0; i < reversedNotation.size(); i++)
	{
		switch (reversedNotation[i].type)
		{
		case WordType::NotIdentified:
			break;

		case WordType::Number:
		case WordType::Variable:
			polishNotation.push_back(reversedNotation[i]);
			break;

		case WordType::MathOperator:
			if (mathOperatorStack.size() == 0)
			{
				mathOperatorStack.push(reversedNotation[i]);
			}
			else
			{
				if (operatorWeightsMap.at(mathOperatorStack.top().data[0]) <= operatorWeightsMap.at(reversedNotation[i].data[0]))
				{
					mathOperatorStack.push(reversedNotation[i]);
				}
				else
				{
					while (operatorWeightsMap.at(mathOperatorStack.top().data[0]) > operatorWeightsMap.at(reversedNotation[i].data[0]))
					{
						polishNotation.push_back(mathOperatorStack.top());
						mathOperatorStack.pop();
						if (mathOperatorStack.empty())
						{
							break;
						}
					}
					i--; // back to the beginning of the case math operator
				}
			}
			break;

		case WordType::RoundBracket:
			if (reversedNotation[i].data[0] == '(')
			{
				mathOperatorStack.push(reversedNotation[i]);
			}
			else
			{
				while (mathOperatorStack.top().data[0] != '(')
				{
					polishNotation.push_back(mathOperatorStack.top());
					mathOperatorStack.pop();
					if (mathOperatorStack.empty())
					{
						break;
					}
				}
				if (!mathOperatorStack.empty())
				{
					mathOperatorStack.pop(); // pop '('
				}

			}
			break;

		default:
			break;
		}
	}

	while (!mathOperatorStack.empty())
	{
		polishNotation.push_back(mathOperatorStack.top());
		mathOperatorStack.pop();
	}

	reverse(polishNotation);

	return polishNotation;
}

std::vector<PolishNotation::Word> PolishNotation::standartToReversePolish(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[standartToReversePolish]: Empty notation in arguments");
	}

	std::vector<Word> reversePolishNotation;
	std::stack<Word> mathOperatorStack;

	for (size_t i = 0; i < notation.size(); i++)
	{
		switch (notation[i].type)
		{
		case WordType::NotIdentified:
			break;

		case WordType::Number:
		case WordType::Variable:
			reversePolishNotation.push_back(notation[i]);
			break;

		case WordType::MathOperator:
			if (mathOperatorStack.size() == 0)
			{
				mathOperatorStack.push(notation[i]);
			}
			else
			{
				if (operatorWeightsMap.at(mathOperatorStack.top().data[0]) < operatorWeightsMap.at(notation[i].data[0]))
				{
					mathOperatorStack.push(notation[i]);
				}
				else
				{
					while (operatorWeightsMap.at(mathOperatorStack.top().data[0]) >= operatorWeightsMap.at(notation[i].data[0]))
					{
						reversePolishNotation.push_back(mathOperatorStack.top());
						mathOperatorStack.pop();
						if (mathOperatorStack.empty())
						{
							break;
						}
					}
					i--; // back to the beginning of the case math operator
				}
			}
			break;

		case WordType::RoundBracket:
			if (notation[i].data[0] == '(')
			{
				mathOperatorStack.push(notation[i]);
			}
			else
			{
				while (mathOperatorStack.top().data[0] != '(')
				{
					reversePolishNotation.push_back(mathOperatorStack.top());
					mathOperatorStack.pop();
					if (mathOperatorStack.empty())
					{
						break;
					}
				}
				mathOperatorStack.pop(); // pop '('
			}
			break;

		default:
			break;
		}
	}

	while (!mathOperatorStack.empty())
	{
		reversePolishNotation.push_back(mathOperatorStack.top());
		mathOperatorStack.pop();
	}

	return reversePolishNotation;
}


// The code below is protected by my copyright.
// Use and distribution are not permitted.
std::vector<PolishNotation::Word> PolishNotation::polishToStandart(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[polishToStandart]: Empty notation in arguments");
	}

	std::vector<Word> tmpNotation(notation);

	std::string newData;

	long long mathOperatorIndex = 0;

	while (tmpNotation.size() != 1)
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, false);
		if (mathOperatorIndex == -1)
		{
			break;
		}

		if (mathOperatorIndex + 2 < tmpNotation.size())
		{
			newData += "(";
			newData += tmpNotation[mathOperatorIndex + 1].data;
			newData += tmpNotation[mathOperatorIndex].data;
			newData += tmpNotation[mathOperatorIndex + 2].data;
			newData += ")";
			tmpNotation[mathOperatorIndex].data = newData;
			tmpNotation[mathOperatorIndex].type = WordType::Variable;

			newData.clear();
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex + 2);
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex + 1);
		}
		else
		{
			throw std::exception("[polishToStandart]: An unexpected error occurred");
		}
	}

	tmpNotation = stringToWordVector(tmpNotation[0].data);
	clearExtraBrackets(tmpNotation);

	return tmpNotation;
}

// The code below is protected by my copyright.
// Use and distribution are not permitted.
std::vector<PolishNotation::Word> PolishNotation::polishToReversePolish(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[polishToReversePolish]: Empty notation in arguments");
	}

	std::vector<Word> tmpNotation(notation);

	std::string newData;

	long long mathOperatorIndex = 0;

	while (tmpNotation.size() != 1)
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, false);
		if (mathOperatorIndex == -1)
		{
			break;
		}

		if (mathOperatorIndex + 2 < tmpNotation.size())
		{
			newData += tmpNotation[mathOperatorIndex + 1].data;
			newData += ' ';
			newData += tmpNotation[mathOperatorIndex + 2].data;
			newData += tmpNotation[mathOperatorIndex].data;

			tmpNotation[mathOperatorIndex].data = newData;
			tmpNotation[mathOperatorIndex].type = WordType::Variable;

			newData.clear();
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex + 2);
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex + 1);
		}
		else
		{
			throw std::exception("[polishToReversePolish]: An unexpected error occurred");
		}
	}

	return stringToWordVector(tmpNotation[0].data);
}


// The code below is protected by my copyright.
// Use and distribution are not permitted.
std::vector<PolishNotation::Word> PolishNotation::reversePolishToStandart(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[reversePolishToStandart]: Empty notation in arguments");
	}

	std::vector<Word> tmpNotation(notation);

	std::string newData;

	long long mathOperatorIndex = 0;

	while (tmpNotation.size() != 1)
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, true);
		if (mathOperatorIndex == -1)
		{
			break;
		}

		if (mathOperatorIndex - 2 >= 0)
		{
			newData += "(";
			newData += tmpNotation[mathOperatorIndex - 2].data;
			newData += tmpNotation[mathOperatorIndex].data;
			newData += tmpNotation[mathOperatorIndex - 1].data;
			newData += ")";

			tmpNotation[mathOperatorIndex - 2].data = newData;

			newData.clear();
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex);
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex - 1);
		}
		else
		{
			throw std::exception("[reversePolishToStandart]: An unexpected error occurred");
		}
	}

	tmpNotation = stringToWordVector(tmpNotation[0].data);
	clearExtraBrackets(tmpNotation);

	return tmpNotation;
}

// The code below is protected by my copyright.
// Use and distribution are not permitted.
std::vector<PolishNotation::Word> PolishNotation::reversePolishToPolish(const std::vector<Word>& notation) const {
	if (notation.empty())
	{
		throw std::exception("[reversePolishToPolish]: Empty notation in arguments");
	}

	std::vector<Word> tmpNotation(notation);

	std::string newData;

	long long mathOperatorIndex = 0;

	const size_t lol = tmpNotation.size();

	while (tmpNotation.size() != 1)
	{
		mathOperatorIndex = findWordIndexByType(tmpNotation, WordType::MathOperator, true);
		if (mathOperatorIndex == -1)
		{
			break;
		}

		if (mathOperatorIndex - 2 < tmpNotation.size())
		{
			newData += tmpNotation[mathOperatorIndex].data;
			newData += tmpNotation[mathOperatorIndex - 2].data;
			newData += ' ';
			newData += tmpNotation[mathOperatorIndex - 1].data;

			tmpNotation[mathOperatorIndex - 2].data = newData;
			tmpNotation[mathOperatorIndex - 2].type = WordType::Variable; //WARNING: Extra code

			newData.clear();
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex);
			tmpNotation.erase(tmpNotation.begin() + mathOperatorIndex - 1);
		}
		else
		{
			throw std::exception("[reversePolishToPolish]: An unexpected error occurred");
		}
	}

	tmpNotation = stringToWordVector(tmpNotation[0].data);

	return tmpNotation;
}


//       -CALCULATIONS-       //

long double PolishNotation::getMathValue(const Word& word) const {
	if (word.data.empty())
	{
		throw std::exception("[getMathValue]: The word data is empty");
	}

	if (word.type == WordType::NotIdentified || word.type == WordType::MathOperator || word.type == WordType::RoundBracket)
	{
		throw std::exception("[getMathValue]: The word is of an invalid type");
	}

	long double value;

	std::istringstream iss;

	std::map<std::string, long double>::iterator it;

	if (word.type == WordType::Number)
	{
		iss.str(word.data);
		iss >> value;
		return value;
	}
	if (word.type == WordType::Variable)
	{
		it = variableMap.find(word.data);

		if (it != variableMap.end())
		{
			return it->second;
		}
		else
		{
			throw std::exception("[getMathValue]: Variable not set");
		}
	}

	throw std::exception("[getMathValue]: An unexpected error occurred");
}

std::vector<PolishNotation::MathWord> PolishNotation::wordToMathWord(std::vector<Word> notation) const {
	if (notation.empty())
	{
		throw std::exception("[wordToMathWord]: The word vector is empty");
	}

	std::vector<MathWord> mathNotation;

	for (size_t i = 0; i < notation.size(); i++)
	{
		mathNotation.push_back(wordToMathWord(notation[i]));
	}

	return mathNotation;
}

PolishNotation::MathWord PolishNotation::wordToMathWord(Word word) const {
	if (word.data.empty())
	{
		throw std::exception("[wordToMathWord]: The word is empty");
	}

	switch (word.type)
	{
	case WordType::Number:
	case WordType::Variable:
		return MathWord(getMathValue(word), WordType::Number);
		break;

	case WordType::MathOperator:
		return MathWord(word.data, WordType::MathOperator);
		break;

	case WordType::RoundBracket:
		return MathWord(word.data, WordType::RoundBracket);
		break;

	default:
		throw std::exception("[wordToMathWord]: The word is of an invalid type");
		break;
	}
}
