#include "LinkedList.h"



static LinkedList Construct(SizeType datatypeSize)
{
	LinkedList ret;
	ret.header			= NullPointer;
	ret.count			= 0;
	ret.datatypeSize	= datatypeSize;

	return ret;
}

static Void Destruct(LinkedList* instance)
{
	LinkedListNode* node = instance->header;
	while (node)
	{
		LinkedListNode* next = node->next;

		FLinkedListNode.Destruct(node);
		node = next;
	}
}

static Void Add(LinkedList* instance, LinkedListNode* node)
{
	if ( instance->count == 0 )
	{
		instance->header = node;
	}
	else
	{
		FLinkedList.IndexAt( instance, instance->count - 1 )->next = node;
	}

	instance->count++;
}

static Void InsertAt(LinkedList* instance, SizeType index, LinkedListNode* node)
{
	if ( index >= instance->count )
		return;

	if ( index == 0 )
	{
		node->next			= instance->header;
		instance->header	= node;
	}
	else
	{
		LinkedListNode* previous = FLinkedList.IndexAt( instance, index - 1 );

		node->next		= previous->next;
		previous->next	= node;
	}

	instance->count++;
}

static Void RemoveAt(LinkedList* instance, SizeType index)
{
	if ( index >= instance->count )
		return;

	if ( index == 0 )
	{
		LinkedListNode* removed	= instance->header;
		instance->header		= removed->next;

		FLinkedListNode.Destruct(removed);
	}
	else
	{
		LinkedListNode* previous	= FLinkedList.IndexAt( instance, index - 1 );
		LinkedListNode* removed		= previous->next;

		previous->next = removed->next;

		FLinkedListNode.Destruct(removed);
	}

	instance->count--;
}

static Void Clear(LinkedList* instance)
{
	FLinkedListNode.Destruct(instance);

	instance->header	= NullPointer;
	instance->count		= 0;
}

static LinkedListNode* IndexAt(LinkedList* instance, SizeType index)
{
	LinkedListNode* ret = instance->header;

	for ( SizeType iteration = 1; iteration <= index; iteration++ )
		if ( !ret )
			return NullPointer;
		else
			ret = ret->next;

	return ret;
}


struct FLinkedList FLinkedList =
{
	Construct,
	Destruct,
	Add,
	InsertAt,
	RemoveAt,
	Clear,
	IndexAt,
};