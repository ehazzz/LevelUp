#include "AsciiGlyphInfo.h"




struct FAsciiGlyphInfo FAsciiGlyphInfo =
{
	32,
	95,
	8,
	16,
	
	/**
	* Note:
	* No problem here(some IDEs maybe throw some errors(as Visual Studio))
	*/
	{
		#include "AsciiLibrary.h"
	}
};
