#include "LinkedListNode.h"



static LinkedListNode* Construct(SizeType datatypeSize)
{
	const SizeType Offset = sizeof(LinkedListNode);


	LinkedListNode* ret	= CAST( LinkedListNode*, FAllocator.Allocate( Offset + datatypeSize ) );
	ret->next			= NullPointer;
	ret->element		= CAST(Byte*, ret) + Offset;

	return ret;
}

static Void Destruct(LinkedListNode* instance)
{
	FAllocator.Release(instance);
}


struct FLinkedListNode FLinkedListNode =
{
	Construct,
	Destruct,
};