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
	class ImprovedList
	{
	public:
		ImprovedList() {
			first = nullptr;
			last = nullptr;
			listSize = 0;
		}
		~ImprovedList() {
			listElement<T>* tmpNode = last;
			for (size_t i = 0; i < listSize; i++)
			{
				if (tmpNode->prev != nullptr) {
					tmpNode = tmpNode->prev;
					delete tmpNode->next;
				}
			}
		}
		
		size_t size() { 
			return listSize; 
		}

		T& operator[](const size_t index) {
			if (index < 0 || index >= listSize)
				throw MZ_il::failure("Bad index", index);
			
			listElement<T>* tmpNode = first;
			
			for (size_t i = 0; i < index; i++)
				tmpNode = tmpNode->next;

			return tmpNode->data;
		}
		
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

		//only for existing list indexes
		void insert_byIndex(T& obj, const size_t index) {
			if (index < 0 || index >= listSize)
				throw MZ_il::failure("Bad index", index);

			listElement<T>* tmpNode = first;

			for (size_t i = 0; i < index; i++)
				tmpNode = tmpNode->next;

			if (tmpNode->prev != nullptr) {
				tmpNode->prev->next = new listElement<T>(obj, tmpNode->prev);
				tmpNode->prev->next->next = tmpNode;
				tmpNode->prev = tmpNode->prev->next;
			}
			else {
				tmpNode->prev = new listElement<T>(obj, nullptr, tmpNode);

				if (tmpNode == first) //check is not obligatory
					first = tmpNode->prev;
			}

			listSize++;
		};

		//returns -1 if not found
		long long find(T& obj) {
			listElement<T>* tmpNode = first;

			for (size_t i = 0; i < listSize; i++) {
				if (tmpNode->data == obj)
					return i;
				tmpNode = tmpNode->next;
			}

			return -1;
		};

		void clearList() {
			listElement<T>* tmpNode = last;
			for (size_t i = 0; i < listSize; i++)
			{
				if (tmpNode->prev != nullptr) {
					tmpNode = tmpNode->prev;
					delete tmpNode->next;
				}
			}
			listSize = 0;
			first = nullptr;
			last = nullptr;
		};

		void pop_node(const size_t nodeIndex) {
			if (nodeIndex < 0 || nodeIndex >= listSize)
				throw MZ_il::failure("Bad node index", nodeIndex);
			
			listElement<T>* tmpNode = first;

			for (size_t i = 0; i < nodeIndex; i++)
				tmpNode = tmpNode->next;

			if (tmpNode == first)
				first = tmpNode->next;
			if (tmpNode == last)
				last = tmpNode->prev;

			delete tmpNode;

			listSize--;
		}

		void swapNodes(size_t firstNodeIndex, size_t secondNodeIndex) {
			if (firstNodeIndex < 0 || firstNodeIndex >= listSize)
				throw MZ_il::failure("Bad first index", firstNodeIndex);
			if (secondNodeIndex < 0 || secondNodeIndex >= listSize)
				throw MZ_il::failure("Bad second index", secondNodeIndex);
			
			listElement<T>* firstNode;
			listElement<T>* secondNode;
			listElement<T>* currentNode = first;

			T tmpData;

			for (size_t i = 0; i < listSize; i++)
			{
				if (firstNodeIndex == i)
					firstNode = currentNode;
				if (secondNodeIndex == i)
					secondNode = currentNode;
				
				currentNode = currentNode->next;
			}

			if (firstNode == nullptr || secondNode == nullptr)
				throw MZ_il::failure("Something went wrong during the node swapping. (nullptr received)");
			
			tmpData = firstNode->data;
			firstNode->data = secondNode->data;
			secondNode->data = tmpData;
		}

	private:
		listElement<T>* first;
		listElement<T>* last;

		size_t listSize;
	};
}

