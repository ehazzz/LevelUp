#pragma once

#include "Engine.h"

#include <iostream>


using namespace Engine;


inline Void TestCase_LinkedList()
{
	LinkedList<int> list;

	for (int i = 0; i < 10; i++)
	{
		auto node = Allocator::Allocate<LinkedList<int>::Node>(1);

		node->element = i;

		list.Add(node);
	}


	auto other = list;

	{
		//other.RemoveAt(0);

		auto node = Allocator::Allocate<LinkedList<int>::Node>(1);
		node->element = 88;

		other.InsertAt(0, node);
	}

	for (auto node : other)
		std::cout << node->element << "\n";
}