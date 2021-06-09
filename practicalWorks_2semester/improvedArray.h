#pragma once
#include <limits>
#include <exception>

// MZ = marzon, ia = improved array
namespace MZ_ia {
	class failure :public std::exception
	{
	public:
		failure(const char* msg) : std::exception(msg), valueState(0) {};
		failure(const char* msg, const size_t& valueState_) : std::exception(msg), valueState(valueState_) {};
		~failure() {};
		size_t valueState;
	};

	template<class T>
	class ImprovedArray
	{
	public:
		ImprovedArray() {
			arrSize = 0;
			arrCap = computeArrCap(0);
			arr = new T[arrCap];
		};
		ImprovedArray(const size_t arrSize_) {
			arrSize = arrSize_;
			arrCap = computeArrCap(arrSize_);
			arr = new T[arrCap];
		};

		~ImprovedArray() {
			delete[] arr;
		};

		inline size_t size() {
			return arrSize;
		};
		inline size_t capacity() {
			return arrCap;
		};

		T& operator[](const size_t index) {
			if (index < 0 || index >= arrSize)
				throw MZ_ia::failure("Bad index", index);

			return arr[index];
		}

		void push_back(T& obj) {
			if (arrSize < arrCap)
				arr[arrSize++] = obj;
			else
			{
				resize(arrSize + 1);
				arr[arrSize - 1] = obj;
			}
		};
		
		//only for existing array indexes
		void insert_byIndex(T& obj, const size_t index) {
			if (index < 0 || index >= arrSize)
				throw MZ_ia::failure("Bad index", index);

			if (arrSize + 1 > arrCap)
				resize(arrSize + 1);
			else
				arrSize++;

			shiftElements(index, arrSize - 1);
			arr[index] = obj;
		};
		
		void pop_index(const size_t index) {
			if (index < 0 || index >= arrSize)
				throw MZ_ia::failure("Bad index", index);

			if (index != arrSize - 1)
				shiftElements(arrSize - 1, index);

			resize(arrSize - 1);
		};

		//returns -1 if not found
		long long find(T& obj) {
			for (size_t i = 0; i < arrSize; i++)
			{
				if (arr[i] == obj)
					return i;
			}
			return -1;
		};
		
		void resize(size_t newSize) {
			if (newSize < 0 || newSize >= std::numeric_limits<size_t>::max())
				throw MZ_ia::failure("Bad new array size", newSize);
			if (newSize == arrSize)
				return;

			T* tmpArr = nullptr;

			if (newSize < arrSize) {
				if (static_cast<double>(computeArrCap(newSize)) / arrCap >= 0.90)
					arrSize = newSize;
				else
				{
					tmpArr = new T[computeArrCap(newSize)];

					for (size_t i = 0; i < newSize; i++)
						tmpArr[i] = arr[i];

					delete[] arr;

					arr = tmpArr;
					arrSize = newSize;
					arrCap = computeArrCap(newSize);
				}
			}
			else { // newSize > arrSize
				if (newSize <= arrCap)
					arrSize = newSize;
				else
				{
					tmpArr = new T[computeArrCap(newSize)];

					for (size_t i = 0; i < arrSize; i++)
						tmpArr[i] = arr[i];

					delete[] arr;

					arr = tmpArr;
					arrSize = newSize;
					arrCap = computeArrCap(newSize);
				}
			}
		};

	private:
		T* arr;
		size_t arrSize;
		size_t arrCap;

		void shiftElements(size_t startIndex, size_t stopIndex) {
			if (startIndex < 0 || startIndex >= arrSize)
				throw MZ_ia::failure("Bad index", startIndex);
			if (stopIndex < 0 || stopIndex >= arrSize)
				throw MZ_ia::failure("Bad index", stopIndex);
			if (startIndex == stopIndex)
				return;

			if (startIndex < stopIndex)
				for (size_t i = stopIndex; i > startIndex; i--)
					arr[i] = arr[i - 1];
			else
				for (size_t i = stopIndex; i < startIndex; i++)
					arr[i] = arr[i + 1];
		};

		size_t computeArrCap(size_t arrSize_) {
			if (arrSize_ < 200)
				return arrSize_ + 20;

			return static_cast<size_t>(static_cast<double>(arrSize_) * 1.10);
		};
	};
}