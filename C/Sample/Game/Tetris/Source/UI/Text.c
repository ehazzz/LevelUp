#include "Text.h"



static Void Render(Serface* serface, const Char8* inAsciiString, const Vector2* position, const Color* color)
{
	//ASSERT( serface->datatypeSize == sizeof(Color) );


	SizeType count = FUnicode.GetUTF8ElementCount(inAsciiString, 0x00);

	// check bound
	{
		if ( position->x + FAsciiGlyphInfo.Width * count >= serface->width )
			return;

		if ( position->y + FAsciiGlyphInfo.Height >= serface->height )
			return;
	}

	for ( SizeType index = 0; index < count; index++ )
	{
		Char8 codepoint = inAsciiString[index];

		if ( codepoint < 32 )
			continue;

		const Byte* Mask = FAsciiGlyphInfo.Masks[ codepoint - FAsciiGlyphInfo.CodePointOffset ];

		for( SizeType column = 0; column < FAsciiGlyphInfo.Height; column++ )
			for( SizeType row = 0; row < FAsciiGlyphInfo.Width; row++ )
				if ( ( Mask[column] >> row ) & 0x01 )
				{
					SizeType x = position->x + FAsciiGlyphInfo.Width * index + row;
					SizeType y = position->y + column;

					FSerface.SetValue(serface, x, y, color);
				}
	}
}


struct FText FText =
{
	Render,
};
