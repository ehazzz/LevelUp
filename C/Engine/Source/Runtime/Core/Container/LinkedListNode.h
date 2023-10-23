#pragma once

#include "../System/Allocator.h"
#include "../System/Memory.h"



typedef struct LinkedListNode LinkedListNode;


struct LinkedListNode
{
	LinkedListNode*	next;
	Void*			element;
};


struct FLinkedListNode
{
	LinkedListNode*(*Construct)(SizeType datatypeSize);
	Void(*Destruct)(LinkedListNode* instance);
};


extern ENGINE_API struct FLinkedListNode FLinkedListNode;