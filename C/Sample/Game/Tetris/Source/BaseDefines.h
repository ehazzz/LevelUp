#pragma once

#include "Engine.h"



enum
{
	GridSize = 30,

	GridsInX = 10,
	GridsInY = 20,

	TotalGrids = GridsInX * GridsInY,

	ElementGrids = 4,
};


static const SizeType FrameWidth	= GridSize * GridsInX;
static const SizeType FrameHeight	= GridSize * GridsInY;

static const RealType AFrameDuration = 0.5;


typedef enum
{
	/**
	* +--- rotation point
	* |	 [x]
	* +->[x][x]
	*	 [x]
	*/
	Element_A,
	/**
	* [x][x]
	*    [x][x]
	*	  A
	*	  +--- rotation point
	*/
	Element_B,
	/**
	* @remark 
	* No rotation
	* 
	* [x][x]
	* [x][x]
	*/
	Element_C,
	/**
	* [x]
	* [x]
	* [x] <--- rotation point
	* [x]
	*/
	Element_D,
	/**
	*       [x]
	* [x][x][x] <--- rotation point
	*/
	Element_E,

	Element_Count,
}
Element;


typedef enum
{
	ElementOrientation_Degree0,
	ElementOrientation_Degree90,
	ElementOrientation_Degree180,
	ElementOrientation_Degree270,

	ElementOrientation_Count,
}
ElementOrientation;


extern const Vector2 APoints[ 4 * 4 ];
extern const Vector2 BPoints[ 4 * 4 ];
extern const Vector2 CPoints[ 4 * 1 ];
extern const Vector2 DPoints[ 4 * 4 ];
extern const Vector2 EPoints[ 4 * 4 ];
