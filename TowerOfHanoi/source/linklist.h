#ifndef LINKLIST_H
#define LINKLIST_H

/*
Project: Link List + Stack
Author: Lloyd Jay A. Edradan
Date: 10/29/20

You can use this as an array or a stack overloaded, operators are already provided




Ex:
#include <iostream>
#include "linklist.h"

int main()
{

	//It works similarly to std::vector
	LinkList<int> MyData;
	
	MyData.push_back(5);

	//You can use the "[]" operator to access data from the linklist
	std::cout << MyData[0];



	return 0;
}

*/


template <typename T>
struct Node
{
	Node * next = nullptr;
	T val;

	Node(Node * nxt, T nval);
};

template <typename T>
class LinkList
{
private:
	Node<T> * nodes = nullptr;
	Node<T> * iterator = nullptr;
	short iteratorindex = 0;
	unsigned int _size = 0;

public:




private:
	

public:
	LinkList();
	LinkList(unsigned int SetSize);
	LinkList(const LinkList<T>&);


	void push_back(T data);
	void pop_back();
	void remove(unsigned int index);
	void add(T value, unsigned int index);
	void free();
	void enqueue(T data);
	unsigned int size();
	
	
	
	T& operator[](unsigned int index);

};







template<typename T>
inline void LinkList<T>::enqueue(T data)
{

	if (LinkList<T>::nodes == nullptr)
	{
		this->nodes = new Node<T>(nullptr, data);
		_size++;
		iterator = nodes;
		return;
	}

	Node<T> * newnode = new Node<T>(nodes, data);
	nodes = newnode;
	_size++;

}






template<typename T>
inline Node<T>::Node(Node * ptr, T data)
{
	Node::next = ptr;
	Node::val = data;
}

template <typename T>
inline LinkList<T>::LinkList()
{
	this->nodes = nullptr;
}

template <typename T>
inline LinkList<T>::LinkList(unsigned int SetSize)
{
	for (int i = 0; i < SetSize; i++)
	{
		push_back(T());
	}

}

template <typename T>
inline LinkList<T>::LinkList(const LinkList<T>& list)
{
	if (list._size > 0)
	{
		Node<T> * nCurrentNode = list.nodes;
		for (unsigned int i = 0; i < list._size; i++)
		{

			push_back(nCurrentNode->val);
			if (nCurrentNode->next != nullptr)
				nCurrentNode = nCurrentNode->next;
		}

	}

}







template<typename T>
inline T& LinkList<T>::operator[](unsigned int index)
{

	if (index > _size - 1)return nodes->val;


	Node<T> * temp = nodes;

	if (index >= iteratorindex)
	{
		for (int i = iteratorindex; i < index; i++)
		{
			if (temp->next != nullptr)
			{
				temp = temp->next;
			}
			else
			{
				iterator = temp;
				iteratorindex = i;
			}
		}
	}
	else
	{
		for (int i = 0; i < index; i++)
		{
			if (temp->next != nullptr)
			{
				temp = temp->next;
			}
			else
			{
				iterator = temp;
				iteratorindex = i;
			}

		}
	}

	
	return temp->val;
}


template <typename T>
inline void LinkList<T>::push_back(T data)
{
	typedef  LinkList<T> Link;
	if (Link::_size < 1)
	{
		Link::nodes = new Node<T>(nullptr, data);
		Link::_size++;
	}
	else
	{
		Node<T> * temp = Link::nodes;


		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = new Node<T>(nullptr, data);
		Link::_size++;
	}


}

/*This can also be used as a "dequeue" */
template <typename T>
inline void LinkList<T>::pop_back()
{
	if (_size < 1)return;
	else if (_size == 1)
	{
		delete nodes;
		nodes = nullptr;
		_size--;
	}
	else
	{
		Node<T> * temp = nodes;
		while (temp->next->next != nullptr)
		{
			temp = temp->next;
		}
		delete temp->next;
		temp->next = nullptr;
		_size--;
	}
}

template <typename T>
inline void LinkList<T>::add(T value, unsigned int index)
{
	if (index > _size - 1)return;
	if (_size < 1)
	{
		nodes = new Node<T>(nullptr, value);
		_size++;
	}
	else
	{
		Node<T> * temp = nodes;
		for (int i = 0; i < index - 1; i++)
		{
			if (temp->next != nullptr)
				temp = temp->next;
		}


		if (temp->next != nullptr)
		{
			temp->next = new Node<T>(temp->next, value);
			_size++;
		}
		else
			push_back(value);
	}

}



template <typename T>
inline void LinkList<T>::free()
{
	int sz = _size;
	for (int i = 0; i < sz; i++)
	{
		pop_back();
	}
}




template <typename T>
inline unsigned int LinkList<T>::size() { return _size; }


template <typename T>
inline void LinkList<T>::remove(unsigned int index)
{
	if (index > _size - 1)return;
	if (index < 1)
	{
		if (nodes->next != nullptr)
		{
			Node<T> * temp = nodes->next;
			delete nodes;
			nodes = temp;
			_size--;
		}

	}
	else
	{
		Node<T> * temp = nodes;
		for (int i = 0; i < index - 1; i++)
		{
			if (temp->next != nullptr)
				temp = temp->next;
		}


		if (temp->next->next != nullptr)
		{
			Node<T> * temp2 = temp->next;
			temp->next = temp->next->next;
			delete temp2;
			size--;
		}
		else
			pop_back();
	}


}




#endif // !LINKLIST_H







