#pragma once

#include "AsciiGlyphInfo.h"
#include "Serface.h"



struct FText
{
	Void(*Render)(Serface* serface, const Char8* inAsciiString, const Vector2* position, const Color* color);
};

extern ENGINE_API struct FText FText;