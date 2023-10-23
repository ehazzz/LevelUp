#include "Path.h"



namespace Engine
{
    U8String Path::Combine(const U8String& A, const U8String& B)
    {
        auto IsNormalized = [](const U8String& path)
            {
                auto last = path[ path.GetCount() - 1 ];

                if (
                    last == '\\' ||
                    last == '/'
                    )
                    return true;

                return false;
            };

        if ( !IsNormalized(A) )
            return A + '/' + B;

        return A + B;
    }
}