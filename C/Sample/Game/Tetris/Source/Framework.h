#pragma once

#include "BaseDefines.h"

#include "UI/Text.h"

#include <stdlib.h>



typedef struct
{
	Element				Current;
	Element				Next;
	ElementOrientation	CurrentOrientation;
	ElementOrientation	NextOrientation;

	Int32		positionX;
	Int32		positionY;
	RealType	timeScale;
	Bool		Mask[TotalGrids];
	SizeType	score;

	Bool isPause;
	Bool isGameOver;
}
GameStatus;


Void GameInit(Void* userParam);
Void GameUpdate(RealType deltaTime);
Void GameRender(Serface* serface);


inline Element GetRandomElement()
{
	return rand() % Element_Count;
}

inline ElementOrientation GetRandomElementOrientation()
{
	return rand() % ElementOrientation_Count;
}

inline Bool PointIsInside(Int32 x, Int32 y)
{
	if ( x < 0 || x >= GridsInX )
		return False;

	if ( y < 0 || y >= GridsInY )
		return False;

	return True;
}

inline const Vector2* GetElementPoints(Element element, ElementOrientation orientation)
{
	// limit at [0 - 3]
	orientation %= ElementOrientation_Count;

	if ( element == Element_C )
		orientation = 0;


	switch (element)
	{
	case Element_A:
		return &APoints[ 4 * orientation ];
	case Element_B:
		return &BPoints[ 4 * orientation ];
	case Element_C:
		return &CPoints[ 4 * orientation ];
	case Element_D:
		return &DPoints[ 4 * orientation ];
	case Element_E:
		return &EPoints[ 4 * orientation ];
	default:
		return NullPointer;
	}
}


inline Void DrawGrid(Serface* serface, Int32 x, Int32 y, SizeType size, SizeType gap, const Color* color)
{
	if ( !PointIsInside(x, y) )
		return;

	x *= size;
	y *= size;

	Int32 xEnd = x + size - gap;
	Int32 yEnd = y + size - gap;

	for ( Int32 xBegin = x; xBegin < xEnd; xBegin++ )
		for ( Int32 yBegin = y; yBegin < yEnd; yBegin++ )
			FSerface.SetValue(serface, xBegin, yBegin, color);
}

inline Void TryMove(GameStatus* status, Int32 xOffset, Int32 yOffset)
{
	const Vector2* points = GetElementPoints( status->Current, status->CurrentOrientation );

	for ( SizeType index = 0; index < ElementGrids; index++ )
	{
		Int32 x = status->positionX + points[index].x + xOffset;
		Int32 y = status->positionY + points[index].y + yOffset;

		// special condition
		if ( y >= GridsInY )
			continue;

		if ( !PointIsInside(x, y) )
			return;
	}

	// update
	status->positionX += xOffset;
	status->positionY += yOffset;

	// cancel(if any points had been covered by status->Mask)
	for ( SizeType index = 0; index < ElementGrids; index++ )
	{
		Int32 x = status->positionX + points[index].x + xOffset;
		Int32 y = status->positionY + points[index].y + yOffset;

		if ( status->Mask[ GridsInX * y + x ] )
		{
			status->positionX -= xOffset;
			break;
		}
	}
}

inline Void TryRotate(GameStatus* status)
{
	// update
	status->CurrentOrientation += 1;

	const Vector2* points = GetElementPoints( status->Current, status->CurrentOrientation );

	// cancel
	for ( SizeType index = 0; index < ElementGrids; index++ )
	{
		Int32 x = status->positionX + points[index].x;
		Int32 y = status->positionY + points[index].y;

		// special condition
		if ( y >= GridsInY )
			continue;

		if ( !PointIsInside(x, y) || status->Mask[ GridsInX * y + x ] )
		{
			status->CurrentOrientation -= 1;
			break;
		}
	}
}

inline Void CreateGameElementAndResetPosition(GameStatus* status)
{
	status->Current				= status->Next;
	status->CurrentOrientation	= status->NextOrientation;

	status->Next			= GetRandomElement();
	status->NextOrientation	= GetRandomElementOrientation();

	status->positionX = GridsInX / 2;
	status->positionY = GridsInY - 1;
}

inline Void TryUpdateMask(GameStatus* status)
{
	const Vector2* points = GetElementPoints( status->Current, status->CurrentOrientation );

	for ( SizeType index = 0; index < ElementGrids; index++ )
	{
		Int32 x = status->positionX + points[index].x;
		Int32 y = status->positionY + points[index].y;

		if ( !PointIsInside(x, y) )
			return;
	}

	Bool isDirty = False;

	for (SizeType index = 0; index < ElementGrids; index++)
	{
		Int32 x = status->positionX + points[index].x;
		Int32 y = status->positionY + points[index].y;

		if ( status->Mask[ GridsInX * ( y - 1 ) + x ] || y == 0 )
		{
			isDirty = True;
			break;
		}
	}

	if (isDirty)
	{
		for ( SizeType index = 0; index < ElementGrids; index++ )
		{
			Int32 y = status->positionY + points[index].y;

			if ( y == GridsInY - 1 )
			{
				status->isGameOver = True;
				return;
			}
		}

		for ( SizeType index = 0; index < ElementGrids; index++ )
		{
			Int32 x = status->positionX + points[index].x;
			Int32 y = status->positionY + points[index].y;

			status->Mask[ GridsInX * y + x ] = True;
		}

		// calculate score
		{
			static Bool temporaryMask[TotalGrids];
			FMemory.Clear( temporaryMask, sizeof(Bool) * TotalGrids );

			SizeType count	= 0;
			SizeType offset	= 0;

			for ( SizeType iteration = 0; iteration < TotalGrids; iteration += GridsInX )
			{
				Bool haveScore = True;

				SizeType end = iteration + GridsInX;
				for ( SizeType begin = iteration; begin < end; begin++ )
					if ( !status->Mask[begin] )
						haveScore = False;

				if (haveScore)
					count++;
				else
				{
					FMemory.Copy( &status->Mask[iteration], temporaryMask + offset, sizeof(Bool) * GridsInX );
					offset += GridsInX;
				}

			}

			status->score += count * 10;
			FMemory.Copy( temporaryMask, status->Mask, sizeof(Bool) * TotalGrids );
		}

		CreateGameElementAndResetPosition(status);
	}
}