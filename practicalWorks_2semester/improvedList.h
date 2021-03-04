#pragma once
#include <exception>

// MZ = marzon, il = improved list
namespace MZ_il
{
	class failure :public std::exception
	{
	public:
		failure(const char* msg) : std::exception(msg), valueState(0) {};
		failure(const char* msg, const size_t& valueState_) : std::exception(msg), valueState(valueState_) {};
		~failure() {};
		size_t valueState;
	};

	template<class T>
	struct listElement
	{
		T data;
		listElement<T>* prev;
		listElement<T>* next;

		listElement() :prev(nullptr), next(nullptr) {}
		listElement(T& data_) :prev(nullptr), next(nullptr), data(data_) {}
		listElement(T& data_, listElement* prev_, listElement* next_) :prev(prev_), next(next_), data(data_) {}
		listElement(T& data_, listElement* prev_) :prev(prev_), next(nullptr), data(data_) {}
		listElement(listElement* prev_, listElement* next_) :prev(prev_), next(next_) {}
		listElement(listElement* prev_) :prev(prev_), next(nullptr) {}
		
		~listElement() {
			if (prev != nullptr)
				prev->next = next;
			if (next != nullptr)
				next->prev = prev;
		}

	};

	template<class T>
	class improvedList
	{
	public:
		improvedList() {
			first = nullptr;
			last = nullptr;
			listSize = 0;
		}
		~improvedList() {}
		
		void push_back(const T& obj) {
			if (listSize == 0)
			{
				first = new listElement<T>;
				first->data = obj;
				last = first;
				listSize++;
			}
			else
			{
				last->next = new listElement<T>(last);
				last = last->next;
				last->data = obj;
				listSize++;
			}
		}

		void pop_node(const size_t nodeIndex) {
			if (nodeIndex < 0 || nodeIndex >= listSize)
				throw MZ_il::failure("Bad node index", nodeIndex);
			
			listElement<T>* tmpNode = first;

			for (size_t i = 0; i < nodeIndex; i++)
				tmpNode = tmpNode->next;

			delete tmpNode;
		}

	private:
		listElement<T>* first;
		listElement<T>* last;

		size_t listSize;
	};
}

