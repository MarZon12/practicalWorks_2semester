// Part of the code is protected by my copyright.
// Use and distribution are not permitted.
// @marzon

// CODE NEEDED REFACTORING

#pragma once
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <stack>
#include <cmath>

/*
* Usage example 1:
* Input: (0,5*-0.2*A)^2
* Output:
* Native notation type: Standart
* Standart notation: ( 0.5 * ( 0 - 0.2 ) * A ) ^ 2
* Polish notation: ^ * * 0.5 - 0 0.2 A 2
* Reverse polish notation: 0.5 0 0.2 - * A * 2 ^
* Calculated: 100
* 
* Usage example 2:
* Input: ^ * * 0.5 - 0 0.2 A 2 = 
* Output:
* Native notation type: Polish notation
* Standart notation: ( 0.5 * ( 0 - 0.2 ) * A ) ^ 2
* Polish notation: ^ * * 0.5 - 0 0.2 A 2
* Reverse polish notation: 0.5 0 0.2 - * A * 2 ^
* Calculated: 100
* 
* Usage example 3:
* Input: 0.5 0 0.2 - * A * 2 ^
* Output:
* Native notation type: Reverse polish notation
* Standart notation: ( 0.5 * ( 0 - 0.2 ) * A ) ^ 2
* Polish notation: ^ * * 0.5 - 0 0.2 A 2
* Reverse polish notation: 0.5 0 0.2 - * A * 2 ^
* Calculated: 100
* 
* Manual check:
* Calculating the value: (0.5 * (-0.2) * A) ^ 2 = (0.5 * (-0.2) * 100) ^ 2 = ( -0.1 * 100 ) ^ 2 = ( -10 ) ^ 2 = 100
* (A = 100: .setFirstUnsetVariable(100);)
*/

class PolishNotation
{
public:
	PolishNotation();
	PolishNotation(const std::string& notation);
	~PolishNotation();


	//       -TYPES-       //

	enum class NotationType
	{
		InvalidNotation,
		Standart,
		PolishNotation,
		ReversePolishNotation
	};

	enum class WordType
	{
		NotIdentified,
		Number,
		Variable,
		MathOperator,
		RoundBracket
	};
	
	struct Word
	{
		Word() : type(WordType::NotIdentified) {}
		Word(const Word& word) : data(word.data), type(word.type) {}
		Word(const std::string& data, const WordType& type) : data(data), type(type) {}

		Word& operator=(const Word& word) {
			data = word.data;
			type = word.type;
			
			return *this;
		}

		std::string data;
		WordType type;
	};

	struct StateWithMessage {
		StateWithMessage() : message("False state & empty message"), state(false) {}
		StateWithMessage(const StateWithMessage& StateWithMessage) : message(StateWithMessage.message), state(StateWithMessage.state) {}
		StateWithMessage(const std::string& message, const bool& state) : message(message), state(state) {}

		StateWithMessage& operator=(const StateWithMessage& StateWithMessage) {
			message = StateWithMessage.message;
			state = StateWithMessage.state;
			
			return *this;
		}

		std::string message;
		bool state;
	};


	//       -SET NOTATION-       //

	void setNativeNotation(const std::string& notation);


	//       -GET NOTATION-       //

	std::vector<Word> getNativeNotation() const;

	NotationType getNativeNotationType() const;

	std::string getNativeNotationTypeStr() const;


	std::vector<Word> getStandartNotation() const;

	std::vector<Word> getPolishNotation() const;

	std::vector<Word> getReversePolishNotation() const;


	static std::string notationToString(std::vector<Word> notation);


	//       -GET STATUS-       //
	
	bool empty() const;

	size_t size() const;

	// read-only, native notation
	Word operator[](size_t i) const;

	bool getCacheState();

	void setCacheState(bool enabled);


	//       -VARIABLES-       //

	StateWithMessage getNameFirstUnsetVariable() const;

	StateWithMessage setFirstUnsetVariable(const long double value) const;


	// Attention: the method has not been tested
	StateWithMessage getNameNextSetVariable() const;
	
	// Attention: the method has not been tested
	StateWithMessage setNextSetVariable(const long double value) const;


	std::map<std::string, long double> getVariableMap() const;

	std::map<std::string, long double>& getVariableMapRef() const;


	//       -CALCULATIONS-       //

	long double getCalculatedNotation() const;


protected:
	//       -TYPES-       //
	
	struct MathWord
	{
		MathWord() {
			value = 0;
			type = WordType::NotIdentified;
		}
		MathWord(const long double& value, const WordType& type) {
			this->value = value;
			this->type = type;
			mathOperator = std::string();
		}
		MathWord(const std::string& mathOperator, const WordType& type) {
			this->type = type;
			this->mathOperator = mathOperator;
			value = 0;
		}
		MathWord(const MathWord& mathWord) {
			value = mathWord.value;
			mathOperator = mathWord.mathOperator;
			type = mathWord.type;
		}

		MathWord& operator=(const MathWord& mathWord) {
			value = mathWord.value;
			mathOperator = mathWord.mathOperator;
			type = mathWord.type;

			return *this;
		}

		long double value;
		std::string mathOperator;
		WordType type;
	};


private:
	std::vector<Word> nativeNotation;
	NotationType nativeNotationType;

	std::map<char, short> operatorWeightsMap;

	mutable std::map<std::string, long double> variableMap;
	mutable std::map<std::string, long double>::iterator variableMapIterator;

	//       -CACHE-       //

	mutable bool cacheEnabled;

	mutable std::vector<Word> cache_standartNotation;
	mutable std::vector<Word> cache_polishNotation;
	mutable std::vector<Word> cache_reversePolishNotation;


	//       -BASIC REUSABLE FUNCTIONS-       //

	// if limIndex is set:
	// findFirst = true: search in the range [limIndex; notation.size()]
	// findFirst = false: search in the range [0; limIndex]
	long long findWordIndexByType(const std::vector<Word>& notation, const WordType& type, bool findFirst, const long long limIndex = -1) const;
	
	long long findWordIndexByType(const std::vector<MathWord>& notation, const WordType& type, bool findFirst, const long long limIndex = -1) const;

	// if limIndex is set:
	// findFirst = true: search in the range [limIndex; notation.size()]
	// findFirst = false: search in the range [0; limIndex]
	long long findWordIndexByData(const std::vector<Word>& notation, const std::string& data, const bool findFirst, const long long limIndex = -1) const;
	

	std::vector<Word> stringToWordVector(const std::string& notation) const;

	void reverse(std::vector<Word>& notation) const;

	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	void clearExtraBrackets(std::vector<Word>& notation) const;

	void standartNotation_handlingNegativeValues(std::vector<Word>& notation);


	//       -VALIDATORS-       //

	NotationType identifyNotation(const std::vector<Word>& notation) const;


	// Return:
	// Message = state description
	// State = false - notation is incorrect; true - notation is correct
	StateWithMessage validateNotation(const std::vector<Word>& notation, const NotationType& notationType) const;

	StateWithMessage validateNotation_Standart(const std::vector<Word>& notation) const;

	StateWithMessage validateNotation_Polish(const std::vector<Word>& notation) const;
	
	StateWithMessage validateNotation_ReversePolish(const std::vector<Word>& notation) const;


	//       -CONVERTERS-       //

	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	std::vector<Word> standartToPolish(const std::vector<Word>& notationN) const;

	std::vector<Word> standartToReversePolish(const std::vector<Word>& notation) const;


	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	std::vector<Word> polishToStandart(const std::vector<Word>& notation) const;

	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	std::vector<Word> polishToReversePolish(const std::vector<Word>& notation) const;


	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	std::vector<Word> reversePolishToStandart(const std::vector<Word>& notation) const;

	// The code below is protected by my copyright.
	// Use and distribution are not permitted.
	std::vector<Word> reversePolishToPolish(const std::vector<Word>& notation) const;


	//       -CALCULATIONS-       //

	long double getMathValue(const Word& word) const;

	std::vector <MathWord> wordToMathWord(std::vector<Word> notation) const;

	MathWord wordToMathWord(Word word) const;
};