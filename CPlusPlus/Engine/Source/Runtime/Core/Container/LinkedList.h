#pragma once

#include "../Mathematics/Math.h"

#include "../System/Allocator.h"



namespace Engine
{
	template<typename T>
	class LinkedList
	{
	public:
		struct Node
		{
			Node*	next;
			T		element;


			Node();
		};


		class Iterator
		{
		public:
			Iterator();
			Iterator(Node* node);
			Iterator(const Iterator& other);


			Node* GetNode();
			const Node* GetNode() const;


			Iterator& operator=(const Iterator& other);

			Iterator& operator++();
			Iterator operator++(Int32);

			Node* operator->();
			Node* operator*();

			Bool operator==(const Iterator& other) const;
			Bool operator!=(const Iterator& other) const;
		private:
			Node* node;
		};


		Iterator begin();
		Iterator end();

		const Iterator begin() const;
		const Iterator end() const;
	public:
		LinkedList();
		LinkedList(const LinkedList<T>& other);
		LinkedList(LinkedList<T>&& other);
		~LinkedList();


		Void Add(Node* node);
		Void InsertAt(SizeType index, Node* node);
		Void RemoveAt(SizeType index);

		Void Clear();


		Node* GetHeader();
		const Node* GetHeader() const;
		SizeType GetCount() const;


		LinkedList<T>& operator=(const LinkedList<T>& other);
		LinkedList<T>& operator=(LinkedList<T>&& other);

		Node* operator[](SizeType index);
		const Node* operator[](SizeType index) const;
	private:
		Void CopyFrom(const LinkedList<T>& instance);
	private:
		Node* header;

		SizeType	count;
	};


#pragma region Details
	template<typename T>
	LinkedList<T>::Node::Node()
		: next(nullptr), element( T() )
	{

	}

	template<typename T>
	LinkedList<T>::Iterator::Iterator()
		: node(nullptr)
	{

	}

	template<typename T>
	LinkedList<T>::Iterator::Iterator(Node* node)
		: node(node)
	{

	}

	template<typename T>
	inline LinkedList<T>::Iterator::Iterator(const Iterator& other)
		: node( other.node )
	{

	}

	template<typename T>
	typename LinkedList<T>::Node* LinkedList<T>::Iterator::GetNode()
	{
		return node;
	}

	template<typename T>
	const typename LinkedList<T>::Node* LinkedList<T>::Iterator::GetNode() const
	{
		return node;
	}

	template<typename T>
	typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator=(const Iterator& other)
	{
		node = other.node;

		return *this;
	}

	template<typename T>
	typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++()
	{
		node = node->next;

		return *this;
	}

	template<typename T>
	typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++(Int32)
	{
		auto ret = node;

		++( *this );

		return ret;
	}

	template<typename T>
	typename LinkedList<T>::Node* LinkedList<T>::Iterator::operator->()
	{
		return node;
	}

	template<typename T>
	typename LinkedList<T>::Node* LinkedList<T>::Iterator::operator*()
	{
		return node;
	}

	template<typename T>
	Bool LinkedList<T>::Iterator::operator==(const Iterator& other) const
	{
		return node == other.node;
	}

	template<typename T>
	Bool LinkedList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return !operator==(other);
	}

	template<typename T>
	typename LinkedList<T>::Iterator LinkedList<T>::begin()
	{
		return header;
	}

	template<typename T>
	typename LinkedList<T>::Iterator LinkedList<T>::end()
	{
		return nullptr;
	}

	template<typename T>
	const typename LinkedList<T>::Iterator LinkedList<T>::begin() const
	{
		return header;
	}

	template<typename T>
	const typename LinkedList<T>::Iterator LinkedList<T>::end() const
	{
		return nullptr;
	}

	template<typename T>
	LinkedList<T>::LinkedList()
		: header(nullptr), count(0)
	{

	}

	template<typename T>
	LinkedList<T>::LinkedList(const LinkedList<T>& other)
	{
		CopyFrom(other);
	}

	template<typename T>
	LinkedList<T>::LinkedList(LinkedList<T>&& other)
	{
		header	= other.header;
		count	= other.count;

		other.count = nullptr;
	}

	template<typename T>
	LinkedList<T>::~LinkedList()
	{
		auto node = header;
		while (node)
		{
			auto next = node->next;

			Allocator::Release(node);
			node = next;
		}
	}

	template<typename T>
	Void LinkedList<T>::Add(Node* node)
	{
		if ( count == 0 )
		{
			header = node;
		}
		else
		{
			( *this )[ count - 1 ]->next = node;
		}

		count++;
	}

	template<typename T>
	Void LinkedList<T>::InsertAt(SizeType index, Node* node)
	{
		if ( index >= count )
			return;

		if ( index == 0 )
		{
			node->next	= header;
			header		= node;
		}
		else
		{
			auto previous = ( *this )[ index - 1 ];

			node->next		= previous->next;
			previous->next	= node;
		}

		count++;
	}

	template<typename T>
	Void LinkedList<T>::RemoveAt(SizeType index)
	{
		if ( index >= count )
			return;

		if ( index == 0 )
		{
			auto removed = header;

			header = header->next;

			Allocator::Release(removed);
		}
		else
		{
			auto previous = ( *this )[ index - 1 ];

			auto removed	= previous->next;
			previous->next	= removed->next;

			Allocator::Release(removed);
		}

		count--;
	}

	template<typename T>
	Void LinkedList<T>::Clear()
	{
		this->~LinkedList();
		count = 0;
	}

	template<typename T>
	typename LinkedList<T>::Node* LinkedList<T>::GetHeader()
	{
		return header;
	}

	template<typename T>
	const typename LinkedList<T>::Node* LinkedList<T>::GetHeader() const
	{
		return header;
	}

	template<typename T>
	SizeType LinkedList<T>::GetCount() const
	{
		return count;
	}

	template<typename T>
	LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
	{
		this->~LinkedList();
		CopyFrom(other);

		return *this;
	}

	template<typename T>
	LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other)
	{
		Swap( &header,	&other.header	);
		Swap( &count,	&other.count	);

		return *this;
	}

	template<typename T>
	typename LinkedList<T>::Node* LinkedList<T>::operator[](SizeType index)
	{
		/*
		auto ret = header;
		for (SizeType iteration = 1; iteration <= index; iteration++)
			if ( !ret )
				return nullptr;
			else
				ret = ret->next;

		return ret;
		*/

		auto ret = header;
		for (SizeType iteration = 1; iteration <= index; iteration++)
			ret = ret->next;

		return ret;
	}

	template<typename T>
	const typename LinkedList<T>::Node* LinkedList<T>::operator[](SizeType index) const
	{
		/*
		auto ret = header;
		for (SizeType iteration = 1; iteration <= index; iteration++)
			if ( !ret )
				return nullptr;
			else
				ret = ret->next;

		return ret;
		*/

		auto ret = header;
		for (SizeType iteration = 1; iteration <= index; iteration++)
			ret = ret->next;

		return ret;
	}

	template<typename T>
	Void LinkedList<T>::CopyFrom(const LinkedList<T>& instance)
	{
		header	= nullptr;
		count	= 0;

		
		for (auto node : instance)
		{
			auto temporary = Allocator::Allocate<Node>(1);

			temporary->next		= nullptr;
			temporary->element	= node->element;

			Add(temporary);
		}
	}
#pragma endregion
}