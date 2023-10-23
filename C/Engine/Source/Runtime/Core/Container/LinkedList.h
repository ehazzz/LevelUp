#pragma once

#include "LinkedListNode.h"



typedef struct
{
	LinkedListNode* header;

	SizeType count;
	SizeType datatypeSize;
}
LinkedList;


struct FLinkedList
{
	LinkedList(*Construct)(SizeType datatypeSize);
	Void(*Destruct)(LinkedList* instance);


	Void(*Add)(LinkedList* instance, LinkedListNode* node);
	Void(*InsertAt)(LinkedList* instance, SizeType index, LinkedListNode* node);
	Void(*RemoveAt)(LinkedList* instance, SizeType index);

	Void(*Clear)(LinkedList* instance);


	LinkedListNode*(*IndexAt)(LinkedList* instance, SizeType index);
};


extern ENGINE_API struct FLinkedList FLinkedList;